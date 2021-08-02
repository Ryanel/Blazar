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
#include "Blazar/Editor/Windows/GameWindow.h"
#include "Blazar/Editor/Windows/LogWindow.h"
#include "Blazar/Editor/Windows/WorldViewer.h"
#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Editor/Windows/InputViewer.h"
#include "Blazar/Editor/Windows/RenderListViewer.h"
#endif

#include "Tracy.hpp"

namespace Blazar {
namespace Editor {
Editor::Editor() {}

void Editor::setup() {
    // Add layers after this one here
    auto& app = Application::get();

    m_windows.push_back(new GameWindow());
    m_windows.push_back(new LogWindow());
    m_windows.push_back(new AssetEditorWindow());
    m_windows.push_back(new FPSWidgetWindowLayer());
    m_windows.push_back(new WorldViewer());
#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
    m_windows.push_back(new RenderListWindowLayer());
    m_windows.push_back(new InputEditorWindow());
#endif
}

void Editor::render() {
    ZoneScoped;
    auto& app = Application::get();
    
    // Main Menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(app.m_name.c_str())) {
            if (ImGui::MenuItem("Exit")) { app.m_Running = false; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
    for (auto* x : m_windows) { x->render(); }
}

}  // namespace Editor
}  // namespace Blazar
