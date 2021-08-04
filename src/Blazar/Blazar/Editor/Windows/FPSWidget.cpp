#include "bzpch.h"

#include "FPSWidget.h"

#include "Blazar/Application.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Tracy.hpp"

namespace Blazar {
namespace Editor {

FPSWidgetWindowLayer::FPSWidgetWindowLayer()
    : EditorWindow("FPS Widget", "FPS Widget", EditorWindowState::FREE_FLOATING) {
    m_useCustomWindow = true;

    deltaTimes.reserve(numEntries);
    for (size_t i = 0; i < numEntries; i++)
    {
        deltaTimes.push_back(0);
    }
}

void FPSWidgetWindowLayer::render(Editor* editor) {
    ZoneScoped;
    auto& app   = Blazar::Application::get();
    float delta = app.m_deltaTime;

    if (currentEntry >= numEntries) { currentEntry = 0; }
    deltaTimes[currentEntry] = delta;
    currentEntry++;

    float average = 0;
    std::for_each(deltaTimes.begin(), deltaTimes.end(), [&average](float n) { average += n; });
    average /= numEntries;

    if (ImGui::BeginMainMenuBar()) {
        float curX = ImGui::GetCursorPosX();
        ImGui::SetCursorPosX((float)app.GetWindow().GetWidth() - 216.0f);

        ImGui::Separator();

        ImGui::Text("fps: %0.2f", 1.0f / average);
        ImGui::Spacing();
        ImGui::Text("dt: %0.2fms", (float)(average * 1000.0f));

        ImGui::SetCursorPosX(curX);
        ImGui::EndMainMenuBar();
    }
}

}  // namespace Editor
}  // namespace Blazar
