#include "DebugLayers.h"

// Log Events

void LogEventsLayer::OnEvent(Blazar::Events::Event& event) { LOG_GAME_TRACE("Event: {}", event.ToString()); }


// FPS Window
void ImGUIFPSWindowLayer::OnImGUIRender() {
    ImGuiIO& io = ImGui::GetIO();
    if (!(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)) {
        float width = 128;
        float padding_x = 8;
        float padding_y = 24;
        float screenWidth = (float)Application::Get().GetWindow().GetWidth();

        float x = screenWidth - width - padding_x;
        float y = padding_y;

        ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, 48), ImGuiCond_Always);
    }

    float delta = Application::Get().m_deltaTime;

    bool frameRateOpen = true;
    if (ImGui::Begin("Framerate", &frameRateOpen,
                     ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::Text("FPS: %0.2f", 1.0f / delta);
        ImGui::Text("Delta: %0.2f ms", (float)(delta * 1000.0f));
    }

    ImGui::End();
}