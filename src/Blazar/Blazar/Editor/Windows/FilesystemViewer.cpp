#include "bzpch.h"

#include <imgui.h>

#include "FilesystemViewer.h"

#include <imgui.h>
#include <cmath>
#include <filesystem>
#include <stack>
#include <unordered_set>

#include "Blazar/Application.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Editor/Editor.h"
#include "Blazar/Editor/EditorCommand.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/ImGui/IconsFontAwesome.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Utility/Path.h"
#include "TextureViewer.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

void TextCentered(std::string text, float ctx_width, float ctx_x) {
    auto windowWidth = ctx_width;
    auto textWidth   = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX(ctx_x + ((windowWidth - textWidth) * 0.5f));
    ImGui::Text(text.c_str());
}

void FilesystemViewer::NavigateUpFolder() {
    if (m_path_heirarchy.size() > 1) { m_path_heirarchy.pop_back(); }
    dirty = true;
}

FilesystemViewer::FilesystemViewer(Editor* editor)
    : EditorWindow(editor, "Editor: Filesystem Viewer", "Filesystem", State::EDITOR_DOCKED) {
    ZoneScoped;
    m_width_min            = 400;
    m_width_expanded       = 600;
    m_expandRemainingSpace = true;

    m_path_heirarchy.push_back("");
    m_path_heirarchy.push_back("Data");
    m_path_heirarchy.push_back("Textures");

    m_texfolder  = Texture2D::Load("/Editor/Textures/folder.png");
    m_texfile    = Texture2D::Load("/Editor/Textures/file.png");
    m_texrefresh = Texture2D::Load("/Editor/Textures/refresh.png");
    m_texgear    = Texture2D::Load("/Editor/Textures/gear.png");

    m_numthumbsCanLoad = 1;
}

void FilesystemViewer::render() {
    ZoneScoped;
    auto& app = Application::get();

    // Draw the menu bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Thumbnails", "", m_optionEnableThumbnails)) {
                m_optionEnableThumbnails = !m_optionEnableThumbnails;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("VFS")) {
            if (ImGui::MenuItem("Garbage Collection")) { ResourceManager::get()->clean(); }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // Breadcrumbs
    DrawBreadCrumbs();

    m_numthumbsCanLoad = m_optionThumbsCanLoad;
    if (dirty) {
        Refresh();
        dirty = false;
    }

    bool renderList = m_state != State::DOCKED_EXPANDED;

    if (renderList) {
        ImGui::NewLine();
        DrawList();
    } else {
        DrawTable();
    }

    // Options
    if (options_open) {
        if (ImGui::Begin("Asset Window Options", &this->options_open)) {
            CImGUI_Header2("Thumbnails");
            ImGui::PushItemWidth(-150.0f);
            ImGui::SliderFloat("Icon Size", &this->m_size, 64, 512);
            ImGui::SliderInt("Load per frame", &this->m_optionThumbsCanLoad, 0, 3);

            ImGui::PopItemWidth();
        }
        ImGui::End();
    }
}

void FilesystemViewer::RenderItem(std::string name, std::string path, bool isDirectory, bool useList) {
    ImTextureID id;  // Texture to show

    // Determine icon to show
    if (!useList) {
        if (!isDirectory) {
            id = m_texfile->data()->imgui_id();

            std::string_view extension = Blazar::Utility::Paths::get_extension(path);
            if (extension.size() > 0) {
                if (extension.compare("png") == 0) {
                    auto* rm = ResourceManager::get();

                    if (m_optionEnableThumbnails) {
                        if (rm->exists(path)) {
                            id = Texture2D::Load(path)->data()->imgui_id();
                        } else {
                            if (m_numthumbsCanLoad > 0) {
                                m_numthumbsCanLoad--;
                                id = Texture2D::Load(path)->data()->imgui_id();
                            }
                        }
                    }
                }
            }
        } else {
            id = m_texfolder->data()->imgui_id();
        }

        // Draw icon
        ImGui::PushID(path.c_str());
        if (ImGui::ImageButton(id, ImVec2(this->m_size, this->m_size), ImVec2(0, 1), ImVec2(1, 0))) {
            if (isDirectory) {
                m_path_heirarchy.push_back(std::string(name));
                dirty = true;
            } else {
                std::string_view extension = Blazar::Utility::Paths::get_extension(path);
                if (extension.size() > 0) {
                    if (extension.compare("png") == 0) { m_editor->window_add_end<TextureViewer>(path); }
                }
            }
        }
        ImGui::PopID();
        TextCentered(name.c_str(), m_size, ImGui::GetCursorPosX());
    } else {
        std::string formattedName = fmt::format("{} {}", (isDirectory ? ICON_FA_FOLDER : ICON_FA_FILE), name);
        if (ImGui::Selectable(formattedName.c_str())) {
            if (isDirectory) {
                m_path_heirarchy.push_back(std::string(name));
                dirty = true;
            } else {
                std::string_view extension = Blazar::Utility::Paths::get_extension(path);
                if (extension.size() > 0) {
                    if (extension.compare("png") == 0) { 
                        m_editor->perform(new OpenWindowWithPathCommand<TextureViewer>(m_editor, path));
                    }
                }
            }
        }
    }
}

void FilesystemViewer::DrawTable() {
    // Render
    float ColumnSize = ImGui::GetWindowWidth() / (m_size + m_padding);
    int   columns    = (int)ceil(ColumnSize);

    if (columns > 64) { columns = 64; }
    if (columns < 2) { columns = 2; }
    columns--;

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    ImGui::SetCursorPosX(m_padding / 2);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetFontSize() * 2));

    if (ImGui::BeginTable("AssetBrowserTable", columns, ImGuiTableFlags_SizingFixedSame, ImVec2(0, 0))) {
        for (size_t i = 0; i < columns; i++) {
            ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_NoResize, m_size);
        }

        ImGui::TableNextColumn();
        for (auto& x : m_current_directories) {
            RenderItem(x.first, x.second, true, false);
            ImGui::TableNextColumn();
        }
        for (auto x : m_current_files) {
            RenderItem(x.first, x.second, false, false);
            ImGui::TableNextColumn();
        }
        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

void FilesystemViewer::DrawList() {
    if (ImGui::BeginListBox("##Files", ImVec2(-1, -1))) {
        for (auto& x : m_current_directories) { RenderItem(x.first, x.second, true, true); }
        for (auto x : m_current_files) { RenderItem(x.first, x.second, false, true); }
        ImGui::EndListBox();
    }
}

void FilesystemViewer::DrawBreadCrumbs() {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);

    ImVec2 iconSize = ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize());

    if (ImGui::Button(ICON_FA_COG)) { options_open = !options_open; }
    ImGui::SameLine();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 4));

    if (m_state == State::DOCKED_EXPANDED) {
        if (ImGui::ArrowButton("Up Directory", ImGuiDir_Up)) { NavigateUpFolder(); }
        ImGui::SameLine();
    }

    int numback = (int)m_path_heirarchy.size() - 1;
    for (size_t i = 0; i < m_path_heirarchy.size(); i++) {
        std::string name = m_path_heirarchy[i];
        if (i == 0) { name = "Root"; }

        if (ImGui::Button(name.c_str())) {
            for (int numbacks = numback - 1; numbacks >= 0; numbacks--) {
                m_path_heirarchy.pop_back();
                dirty = true;
            }
        }
        ImGui::SameLine();
        ImGui::Text("/");
        ImGui::SameLine();
        numback--;
    }

    ImGui::PopStyleVar();
}

void FilesystemViewer::Refresh() {
    ZoneScoped;
    auto&  vfs    = ResourceManager::get()->m_vfs;
    auto   list   = vfs->filelist();
    size_t levels = m_path_heirarchy.size();

    m_current_directories.clear();
    m_current_files.clear();

    // Discovery
    for (auto item : list) {
        std::string currentString = item;
        std::string originalPath  = item;
        bool        matched       = false;
        for (size_t i = 0; i < levels; i++) {
            size_t index = currentString.find('/', 0);

            if (index == std::string::npos) {
                // The file path is smaller
                matched = false;
                break;
            }

            std::string directory_name = std::string(currentString.begin(), currentString.begin() + index);

            if (m_path_heirarchy[i].compare(directory_name) == 0) {
                currentString.erase(currentString.begin(), currentString.begin() + index + 1);
                matched = true;
            } else {
                matched = false;
                continue;
            }
        }

        if (matched) {
            size_t index  = currentString.find('/', 0);
            bool   isFile = index == std::string::npos;
            if (!isFile) {
                currentString.erase(currentString.begin() + index, currentString.end());

                if (m_current_directories.find(currentString) == m_current_directories.end()) {
                    m_current_directories.emplace(currentString, originalPath);
                }

            } else {
                m_current_files.emplace(currentString, originalPath);
            }
        }
    }
}

}  // namespace Editor
}  // namespace Blazar
