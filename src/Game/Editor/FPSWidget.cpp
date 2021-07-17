#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Renderer/Renderer.h"
#include "DebugLayers.h"

#include "FPSWidget.h"
#include "Tracy.hpp"



void FPSWidgetWindowLayer::OnImGUIRender() {
    ZoneScoped;
    auto& app = Application::Get();
    float delta = app.m_deltaTime;

    if (ImGui::BeginMainMenuBar()) {
        float curX = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX(app.GetWindow().GetWidth() - 216);

        ImGui::Separator();

        ImGui::Text("fps: %0.2f", 1.0f / delta);
        ImGui::Spacing();
        ImGui::Text("dt: %0.2fms", (float)(delta * 1000.0f));

        ImGui::SetCursorPosX(curX);
        ImGui::EndMainMenuBar();
    }
}
