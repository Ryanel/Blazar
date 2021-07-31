#include "bzpch.h"

#include "WorldViewer.h"

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Renderer.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {
    WorldViewer::WorldViewer() : EditorWindow("World Viewer")
    {
        m_texgear    = Texture2D::Load("/Editor/Textures/gear.png");

    }
    void WorldViewer::RenderWindow() {
    ZoneScoped;
    ImGUI_MainMenu_Toggle_Simple("Windows", "World Viewer", "", this->m_active, true);

    auto& app = Application::Get();

    if (!this->m_active) { return; }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (ImGui::Begin("World Viewer", &this->m_active)) {
        isFocused = ImGui::IsWindowFocused();

        ImGui::PushStyleColor(
            ImGuiCol_ChildBg,
            ImGui::GetStyleColorVec4((isFocused || isTitleFocused) ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg));
        if (ImGui::BeginChild("##WorldViewerTop", ImVec2(0, 30))) {
            isTitleFocused = ImGui::IsWindowFocused();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);

            if (ImGui::ImageButton((ImTextureID)m_texgear->data()->GetId(), ImVec2(16.0f, 16.0f), ImVec2(0.0f, 1.0f),
                ImVec2(1, 0))) {
                m_options_open = !m_options_open;
            }
            ImGui::SameLine();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

}  // namespace Editor
}  // namespace Blazar
