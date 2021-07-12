#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"

#include "FPSWidget.h"
#include "Tracy.hpp"



void FPSWidgetWindowLayer::OnImGUIRender() {
    ZoneScoped;
    // ImGUI_MainMenu_Toggle_Simple("Windows", "Render Layer", "", this->show, true);

    auto& app = Application::Get();
    float delta = app.m_deltaTime;

    if (ImGui::BeginMainMenuBar()) {
        ImGui::SetCursorPosX(app.GetWindow().GetWidth() - 192);

        ImGui::Separator();

        ImGui::Text("FPS: %0.2f", 1.0f / delta);
        ImGui::Spacing();
        ImGui::Text("Delta: %0.2f", (float)(delta * 1000.0f));

        ImGui::EndMainMenuBar();
    }
}
