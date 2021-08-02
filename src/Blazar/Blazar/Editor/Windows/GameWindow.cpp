#include "bzpch.h"

#include "Blazar/Editor/Windows/GameWindow.h"

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

void GameWindow::render() {
    ZoneScoped;

    auto& app = Application::get();
    ImGUI_MainMenu_Toggle_Simple("Windows", "Game Window", "", app.m_UseEditorWindow, true);

    if (!this->show) { return; }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (app.m_UseEditorWindow) {
        if (ImGui::Begin("Game", &app.m_UseEditorWindow, ImGuiWindowFlags_NoBackground)) {
            auto gameSize = ImGui::GetContentRegionAvail();
            auto gamePos  = ImGui::GetWindowPos();
            app.m_RenderViewport->Set(gamePos.x, gamePos.y, gameSize.x, gameSize.y);

            ImGui::Image(app.m_GameRenderTexture->m_ColorTexture->imgui_id(), gameSize, ImVec2(0, 1),
                         ImVec2(1, 0));
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

}  // namespace Editor
}  // namespace Blazar
