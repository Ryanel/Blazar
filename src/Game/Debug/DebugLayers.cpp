#include "DebugLayers.h"
#include "Blazar/Renderer/Renderer.h"
// Log Events

void LogEventsLayer::OnEvent(Blazar::Events::Event& event) { LOG_GAME_TRACE("Event: {}", event.ToString()); }

// FPS Window
void ImGUIFPSWindowLayer::OnImGUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    if (!(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)) {
        float width = 150;
        float padding_x = 8;
        float padding_y = 24;
        float screenWidth = (float)Application::Get().GetWindow().GetWidth();

        float x = screenWidth - width - padding_x;
        float y = padding_y;

        ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, 102), ImGuiCond_Always);
    }

    float delta = Application::Get().m_deltaTime;

    if (ImGui::Begin("Render Statistics", nullptr,
                     ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize)) {
        ImGui::Text("FPS: %0.2f", 1.0f / delta);
        ImGui::Text("Delta: %0.2f ms", (float)(delta * 1000.0f));
        ImGui::Text("Render Passes: %d", renderer_stats.passesThisFrame);
        ImGui::Text("Draw Calls: %d", renderer_stats.drawCalls);
    }

    ImGui::End();
}
