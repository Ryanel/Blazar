#include "AssetViewer.h"

#include <cmath>
#include <filesystem>
#include <stack>
#include <unordered_set>
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"
#include "Tracy.hpp"

void TextCentered(std::string text, float ctx_width, float ctx_x) {
    auto windowWidth = ctx_width;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX(ctx_x + ((windowWidth - textWidth) * 0.5f));
    ImGui::Text(text.c_str());
}

void AssetEditorWindow::NavigateUpFolder() {
    if (m_path_heirarchy.size() > 1) { m_path_heirarchy.pop_back(); }
    dirty = true;
}

void AssetEditorWindow::RenderItem(std::string name, std::string path, bool isDirectory) {
    ImTextureID id;  // Texture to show

    // Determine icon to show
    if (!isDirectory) {
        id = (ImTextureID)m_texfile.data()->GetId();

        size_t period_index = path.find_last_of(".", 0);

        if (period_index != std::string_view::npos) {
            std::string_view extension(path);
            extension = extension.substr(period_index, extension.size() - period_index);
        }
    } else {
        id = (ImTextureID)m_texfolder.data()->GetId();
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

    if (ImGui::Begin("Assets", &this->show)) {
        // Breadcrumbs
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));

        if (ImGui::BeginChild("##AssetWindowTop", ImVec2(0, 64))) {
            if (ImGui::ArrowButton("Up Directory", ImGuiDir_Up)) { NavigateUpFolder(); }
            ImGui::SameLine();
            if (ImGui::Button("Refresh")) { dirty = true; }
            ImGui::SameLine();

            int numback = m_path_heirarchy.size() - 1;
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

            // Second Line
            ImGui::NewLine();

            ImGui::SliderInt("Size", &this->m_size, 16, 256);
        }
        ImGui::EndChild();

        ImGui::PopStyleColor();

        if (dirty) {
            Refresh();
            dirty = false;
        }

        // Render

        float ColumnSize = ImGui::GetWindowWidth() / (m_size + m_padding);
        int columns = (int)ceil(ColumnSize);
        if (columns > 64) { columns = 64; }
        if (columns < 1) { columns = 1; }

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        if (ImGui::BeginChild("##AssetWindowTitle")) {
            float tableWidth = ImGui::GetWindowWidth() - m_padding;
            ImGui::SetCursorPosX(m_padding / 2);
            if (ImGui::BeginTable("AssetBrowserTable", columns, ImGuiTableFlags_SizingFixedSame, ImVec2(tableWidth, 0),
                                  tableWidth)) {
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

        ImGui::End();
        ImGui::PopStyleVar();
    }
}

void AssetEditorWindow::Refresh() {
    ZoneScoped;
    auto& vfs = ResourceManager::Get()->m_vfs;
    auto list = vfs->filelist();
    int levels = m_path_heirarchy.size();

    m_current_directories.clear();
    m_current_files.clear();

    // Discovery
    for (auto item : list) {
        std::string currentString = item;
        std::string originalPath = item;
        bool matched = false;
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
            size_t index = currentString.find('/', 0);
            bool isFile = index == std::string::npos;
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
