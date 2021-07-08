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
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) { Application::Get().m_Running = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::Begin("Game",nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking)) {
        auto gameSize = ImGui::GetWindowSize();
        auto gamePos = ImGui::GetWindowPos();
        Application::Get().m_EditorGameWindow->x = gamePos.x;
        Application::Get().m_EditorGameWindow->y = gamePos.y;

        Application::Get().m_EditorGameWindow->width = gameSize.x;
        Application::Get().m_EditorGameWindow->height = gameSize.y;
    }

    ImGui::End();
}

void Editor::OnEvent(Blazar::Events::Event& e) {}

}  // namespace Blazar
