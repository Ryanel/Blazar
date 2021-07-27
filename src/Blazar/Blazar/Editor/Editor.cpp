#include "bzpch.h"

#include "Editor.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

#include "Blazar/Application.h"
#include "Blazar/Config.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/Renderer.h"

// Windows
#include "Blazar/Editor/Windows/AssetViewer.h"
#include "Blazar/Editor/Windows/FPSWidget.h"
#include "Blazar/Editor/Windows/LogWindow.h"
#include "Blazar/Editor/Windows/SimulationStatsWindow.h"
#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Editor/Windows/InputViewer.h"
#include "Blazar/Editor/Windows/LayerViewer.h"
#include "Blazar/Editor/Windows/RenderListViewer.h"
#endif

#include "Tracy.hpp"

namespace Blazar {
namespace Editor {
Editor::Editor() : Layer("Editor: Main") { m_UpdatePath = LayerUpdatePath::ImGui; }

void Editor::Setup() {
    // Add layers after this one here
    auto& app = Application::Get();

    m_windows.push_back(new LogWindow());
    m_windows.push_back(new AssetEditorWindow());
    m_windows.push_back(new FPSWidgetWindowLayer());
    m_windows.push_back(new SimulationStatsWindow());
#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
    m_windows.push_back(new RenderListWindowLayer());
    m_windows.push_back(new LayerEditorWindow());
    m_windows.push_back(new InputEditorWindow());
#endif
}

void Editor::OnAttach() {}

void Editor::OnDetached() {}

void Editor::OnImGUIRender() {
    ZoneScoped;
    auto& app = Application::Get();
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) { app.m_Running = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGUI_MainMenu_Toggle_Simple("Windows", "Game Window", "", app.m_UseEditorWindow, true);
#ifdef BLAZAR_CFG_DEV_ENABLE
    ImGUI_MainMenu_Toggle_Simple("[Development]", "ImGui Style Editor", "", m_showImGuiStyleEditor, true);
#endif
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (app.m_UseEditorWindow) {
        if (ImGui::Begin("Game", &app.m_UseEditorWindow, ImGuiWindowFlags_NoBackground)) {
            auto gameSize           = ImGui::GetContentRegionAvail();
            auto gamePos            = ImGui::GetWindowPos();
            app.m_RenderViewport->x = gamePos.x;
            app.m_RenderViewport->y = gamePos.y;

            app.m_RenderViewport->width  = gameSize.x;
            app.m_RenderViewport->height = gameSize.y;

            ImGui::Image((ImTextureID)app.m_GameRenderTexture->m_ColorTexture->GetId(), gameSize, ImVec2(0, 1),
                         ImVec2(1, 0));
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();

    if (m_showImGuiStyleEditor) {
        if (ImGui::Begin("Style Editor", &m_showImGuiStyleEditor)) { ImGui::ShowStyleEditor(); }
        ImGui::End();
    }

    for (auto* x : m_windows) { x->OnImGUIRender(); }
}

}  // namespace Editor
}  // namespace Blazar
