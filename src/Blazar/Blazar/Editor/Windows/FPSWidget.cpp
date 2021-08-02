#include "bzpch.h"

#include "FPSWidget.h"

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

void FPSWidgetWindowLayer::render() {
    ZoneScoped;
    auto& app   = Blazar::Application::get();
    float delta = app.m_deltaTime;

    if (ImGui::BeginMainMenuBar()) {
        float curX = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX((float)app.GetWindow().GetWidth() - 216.0f);

        ImGui::Separator();

        ImGui::Text("fps: %0.2f", 1.0f / delta);
        ImGui::Spacing();
        ImGui::Text("dt: %0.2fms", (float)(delta * 1000.0f));

        ImGui::SetCursorPosX(curX);
        ImGui::EndMainMenuBar();
    }
}

}  // namespace Editor
}  // namespace Blazar
