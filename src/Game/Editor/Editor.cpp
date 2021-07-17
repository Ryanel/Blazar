#include "Editor.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

#include "Blazar/Config.h"
#include "Blazar/ImGui/ImGuiLog.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Renderer.h"
#include "Editor/DebugLayers.h"
#include "Editor/FPSWidget.h"

#include "Tracy.hpp"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Editor/RenderListViewer.h"
#endif

namespace Blazar {

Editor::Editor() : Layer("ImGUI Editor Main") { m_UpdatePath = LayerUpdatePath::Render; }

void Editor::Setup() {
    // Add layers after this one here
    auto& app = Application::Get();
    // app.PushLayer(new ImGUIFPSWindowLayer());
    app.PushLayer(new ImGUIDemoWindowLayer());
    app.PushLayer(new ImGUILogWindowLayer());
    app.PushLayer(new FPSWidgetWindowLayer());

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
    app.PushLayer(new RenderListWindowLayer());
#endif
}

void Editor::OnAttach() {}

void Editor::OnDetached() {}

void Editor::OnUpdate(Blazar::Timestep ts) {}

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
            auto gameSize = ImGui::GetContentRegionAvail();
            auto gamePos = ImGui::GetWindowPos();
            app.m_RenderViewport->x = gamePos.x;
            app.m_RenderViewport->y = gamePos.y;

            app.m_RenderViewport->width = gameSize.x;
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
}

void Editor::OnEvent(Blazar::Events::Event& e) {}

}  // namespace Blazar
