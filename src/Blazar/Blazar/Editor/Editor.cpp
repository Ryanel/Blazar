#include "bzpch.h"

#include <algorithm>
#include "Editor.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

#include "Blazar/Application.h"
#include "Blazar/Config.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/Renderer.h"

// Windows
#include "Blazar/Editor/Windows/FPSWidget.h"
#include "Blazar/Editor/Windows/FilesystemViewer.h"
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

Editor::~Editor() {
    for (auto* x : m_windows) { delete x; }
    m_windows.clear();
}

void Editor::setup() {
    // Add layers after this one here
    auto& app = Application::get();

    m_windows.push_back(new FPSWidgetWindowLayer());
    m_windows.push_back(new GameWindow());
    // m_windows.push_back(new LogWindow());
    window_add_end<WorldViewer>();
    window_add_end<FilesystemViewer>();
}

void Editor::close_window(EditorWindow* window) { m_toClose.push_back(window); }

void Editor::window_move_end(EditorWindow* window)
{
    window->m_editorOrder  = editor_get_highest_order() + 1;
}

void Editor::render() {
    ZoneScoped;
    auto& app      = Application::get();
    bool  selected = false;

    // Main Menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(app.m_name.c_str())) {
            if (ImGui::MenuItem("Exit")) { app.m_Running = false; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) { ImGui::EndMenu(); }
        if (ImGui::BeginMenu("Data")) { ImGui::EndMenu(); }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Logs", "", &selected)) { menu_spawn_unique<LogWindow>(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Input Viewer", "", &selected)) { menu_spawn_unique<InputEditorWindow>(); }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Fake variabl
    int  menu_height = 24;
    auto vp          = app.GetWindow().GetViewport();

    ImGui::SetNextWindowPos(ImVec2(0, menu_height));
    ImGui::SetNextWindowSize(ImVec2(vp->width, vp->height - menu_height));
    ImGui::SetNextWindowBgAlpha(0.0f);
    auto dockID = ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode |
                                                            ImGuiDockNodeFlags_NoDockingInCentralNode |
                                                            ImGuiDockNodeFlags_AutoHideTabBar);

    ImGui::SetNextWindowDockID(dockID, ImGuiCond_Always);
    int flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysHorizontalScrollbar;

    // Now, add any windows that we were suppose to...
    for (auto* x : m_toAdd) { m_windows.push_back(x); }
    m_toAdd.clear();

    int last_draw_sort = this->editor_get_highest_order();

    if (ImGui::Begin("Editor", nullptr, flags)) {
        auto content_region = ImGui::GetContentRegionMax();

        auto editor_sort = [](EditorWindow* x, EditorWindow* y) { return x->m_editorOrder < y->m_editorOrder; };
        std::sort(m_windows.begin(), m_windows.end(), editor_sort);

        int cumulative_width = m_windowPadding;
        int highest_order    = this->editor_get_highest_order();
        for (auto* x : m_windows) {
            ImGui::SetCursorPos(ImVec2(cumulative_width, m_windowPadding));

            bool isLast = highest_order == x->m_editorOrder;

            if (x->m_state != EditorWindow::EditorWindowState::FREE_FLOATING) {
                // Determine window width.
                float width = 12;
                switch (x->m_state) {
                    case EditorWindow::EditorWindowState::EDITOR_DOCKED:
                        width = x->m_width_min;
                        break;
                    case EditorWindow::EditorWindowState::DOCKED_EXPANDED:
                        width = x->m_width_expanded;

                        if (isLast && x->m_expandRemainingSpace) {
                            float amount_left = ImGui::GetWindowWidth() - cumulative_width;
                            if (amount_left > width + m_windowPadding) { width = amount_left - m_windowPadding; }
                        }
                        break;
                    case EditorWindow::EditorWindowState::DOCKED_MINIMIZED:
                        width = 48.0f;
                        break;
                    default:
                        width = x->m_width;
                        break;
                }

                x->m_width = width;
                cumulative_width += width + m_innerWindowPadding;
                x->m_height = content_region.y - (2 * m_windowPadding);
            }

            x->draw_editor_window(this);
        }
    }
    ImGui::End();

    // Now, delete any windows that we were suppose to...
    for (auto* x : m_toClose) {
        m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), x), m_windows.end());
        delete x;
    }

    m_toClose.clear();

    ImGui::ShowStyleEditor();
}

}  // namespace Editor
}  // namespace Blazar
