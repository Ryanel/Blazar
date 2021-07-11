#include "Editor.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Renderer.h"

#include "Blazar/ImGui/ImGuiLog.h"
#include "Editor/DebugLayers.h"

#include "Tracy.hpp"

namespace Blazar {

Editor::Editor() : Layer("ImGUI Editor Main") { m_UpdatePath = LayerUpdatePath::Render; }

void Editor::Setup() {
    // Add layers after this one here
    auto& app = Application::Get();
    app.PushLayer(new ImGUIFPSWindowLayer());
    app.PushLayer(new ImGUIDemoWindowLayer());
    app.PushLayer(new ImGUILogWindowLayer());
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
        if (ImGui::BeginMenu("Windows")) {
            if (ImGui::MenuItem("Game", "", &app.m_UseEditorWindow)) { app.m_UseEditorWindow != app.m_UseEditorWindow; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

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


            ImGui::Image((ImTextureID)app.m_GameRenderTexture->GetColorId(), gameSize, ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

void Editor::OnEvent(Blazar::Events::Event& e) {}

}  // namespace Blazar
