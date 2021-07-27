#include "bzpch.h"

#include <imgui.h>

#include "AssetViewer.h"

#include <imgui.h>
#include <cmath>
#include <filesystem>
#include <stack>
#include <unordered_set>

#include "Blazar/Application.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Utility/Path.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

void TextCentered(std::string text, float ctx_width, float ctx_x) {
    auto windowWidth = ctx_width;
    auto textWidth   = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX(ctx_x + ((windowWidth - textWidth) * 0.5f));
    ImGui::Text(text.c_str());
}

void AssetEditorWindow::NavigateUpFolder() {
    if (m_path_heirarchy.size() > 1) { m_path_heirarchy.pop_back(); }
    dirty = true;
}

AssetEditorWindow::AssetEditorWindow() : EditorWindow("Editor: Asset Window") {
    ZoneScoped;

    m_path_heirarchy.push_back("");
    m_path_heirarchy.push_back("Data");
    m_path_heirarchy.push_back("Shaders");

    m_texfolder  = Texture2D::Load("/Editor/Textures/folder.png");
    m_texfile    = Texture2D::Load("/Editor/Textures/file.png");
    m_texrefresh = Texture2D::Load("/Editor/Textures/refresh.png");
    m_texgear    = Texture2D::Load("/Editor/Textures/gear.png");

    m_numthumbsCanLoad = 1;
}

void AssetEditorWindow::RenderItem(std::string name, std::string path, bool isDirectory) {
    ImTextureID id;  // Texture to show

    // Determine icon to show
    if (!isDirectory) {
        id = (ImTextureID)m_texfile->data()->GetId();

        std::string_view extension = Blazar::Utility::Paths::GetExtension(path);
        if (extension.size() > 0) {
            if (extension.compare("png") == 0) {
                auto* rm = ResourceManager::Get();

                if (m_optionEnableThumbnails) {
                    if (rm->Exists(path)) {
                        id = (ImTextureID)Texture2D::Load(path)->data()->GetId();
                    } else {
                        if (m_numthumbsCanLoad > 0) {
                            m_numthumbsCanLoad--;
                            id = (ImTextureID)Texture2D::Load(path)->data()->GetId();
                        }
                    }
                }
            }
        }
    } else {
        id = (ImTextureID)m_texfolder->data()->GetId();
    }

    // Draw icon
    ImGui::PushID(path.c_str());
    if (ImGui::ImageButton(id, ImVec2(this->m_size, this->m_size), ImVec2(0, 1), ImVec2(1, 0))) {
        if (isDirectory) {
            m_path_heirarchy.push_back(std::string(name));
            dirty = true;
        }
    }
    ImGui::PopID();

    TextCentered(name.c_str(), m_size, ImGui::GetCursorPosX());
}

void AssetEditorWindow::OnImGUIRender() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("Windows", "Assets", "", this->show, true);

    auto& app = Application::Get();

    if (!this->show) { return; }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    m_numthumbsCanLoad = m_optionThumbsCanLoad;

    if (ImGui::Begin("Assets", &this->show)) {
        // Breadcrumbs
        isFocused = ImGui::IsWindowFocused();

        if (isFocused || isChildFocused) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_TitleBgActive));
        } else {
            if (m_numthumbsCanLoad > 1) { m_numthumbsCanLoad = 1; }
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
        }

        if (ImGui::BeginChild("##AssetWindowTop", ImVec2(0, 28))) {
            isChildFocused = ImGui::IsWindowFocused();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);
            if (ImGui::ImageButton((ImTextureID)m_texgear->data()->GetId(), ImVec2(16.0f, 16.0f), ImVec2(0.0f, 1.0f),
                                   ImVec2(1, 0))) {
                options_open = !options_open;
            }
            ImGui::SameLine();

            if (ImGui::ImageButton((ImTextureID)m_texrefresh->data()->GetId(), ImVec2(16.0f, 16.0f), ImVec2(0.0f, 1.0f),
                                   ImVec2(1.0f, 0.0f))) {
                dirty = true;
            }
            ImGui::SameLine();

            if (ImGui::ArrowButton("Up Directory", ImGuiDir_Up)) { NavigateUpFolder(); }
            ImGui::SameLine();

            size_t numback = m_path_heirarchy.size() - 1;
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
        }
        ImGui::EndChild();

        ImGui::PopStyleColor();

        if (dirty) {
            Refresh();
            dirty = false;
        }

        // Render
        float ColumnSize = ImGui::GetWindowWidth() / (m_size + m_padding);
        int   columns    = (int)ceil(ColumnSize);

        if (columns > 64) { columns = 64; }
        if (columns < 2) { columns = 2; }
        columns--;

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        if (ImGui::BeginChild("##AssetWindowTitle")) {
            isChildFocused |= ImGui::IsWindowFocused();
            ImGui::SetCursorPosX(m_padding / 2);
            if (ImGui::BeginTable("AssetBrowserTable", columns, ImGuiTableFlags_SizingFixedSame, ImVec2(0, 0))) {
                for (size_t i = 0; i < columns; i++) {
                    ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_NoResize, m_size);
                }

                ImGui::TableNextColumn();
                for (auto& x : m_current_directories) {
                    RenderItem(x.first, x.second, true);
                    ImGui::TableNextColumn();
                }
                for (auto x : m_current_files) {
                    RenderItem(x.first, x.second, false);
                    ImGui::TableNextColumn();
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
    }
    ImGui::End();
    ImGui::PopStyleVar();

    // Options
    if (options_open) {
        if (ImGui::Begin("Asset Window Options", &this->options_open)) {
            CImGUI_Header1("Options");

            CImGUI_Header2("Thumbnails");

            ImGui::SliderFloat("Icon Size", &this->m_size, 64, 512);
            ImGui::Checkbox("Enable Thumbnails", &this->m_optionEnableThumbnails);
            ImGui::SliderInt("Thumbnails to load per frame", &this->m_optionThumbsCanLoad, 0, 3);
            CImGUI_Header2("Development");
            if (ImGui::Button("Clean")) { ResourceManager::Get()->Clean(); }
        }
        ImGui::End();
    }
}

void AssetEditorWindow::Refresh() {
    ZoneScoped;
    auto&  vfs    = ResourceManager::Get()->m_vfs;
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
