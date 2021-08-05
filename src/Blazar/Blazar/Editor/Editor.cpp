#include "bzpch.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Blazar/Application.h"
#include "Blazar/Config.h"
#include "Blazar/Editor/Windows/FPSWidget.h"
#include "Blazar/Editor/Windows/FilesystemViewer.h"
#include "Blazar/Editor/Windows/GameWindow.h"
#include "Blazar/Editor/Windows/LogWindow.h"
#include "Blazar/Editor/Windows/WorldViewer.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/Renderer.h"
#include "Editor.h"
#include "EditorCommand.h"
#include "Tracy.hpp"
#include "imgui.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Editor/Windows/InputViewer.h"
#include "Blazar/Editor/Windows/RenderListViewer.h"
#endif

namespace Blazar {
namespace Editor {
Editor::Editor() {}

Editor::~Editor() { m_windows.clear(); }

void Editor::setup() {
    // Add layers after this one here
    auto& app = Application::get();

    window_add_end<FPSWidgetWindowLayer>();
    window_add_end<GameWindow>();
    window_add_end<LogWindow>();
    window_add_end<WorldViewer>();
    window_add_end<FilesystemViewer>();
}

void Editor::close_window(EditorWindow* w) { m_toClose.push_back(find_ref_from_list(w, m_windows)); }
void Editor::window_move_end(EditorWindow* window) { window->m_editorOrder = editor_get_highest_order() + 1; }

void Editor::undo() {
    if (m_editorCommandPtr == 0) { return; }

    m_editorCommandPtr--;
    m_editorCommandList[m_editorCommandPtr]->undo();
}

void Editor::redo() {
    if (m_editorCommandPtr < m_editorCommandList.size()) {
        m_editorCommandList[m_editorCommandPtr]->execute();
        m_editorCommandPtr++;
    }
}

void Editor::perform(EditorCommand* c) {
    // Are we at the end of the list?
    if (m_editorCommandPtr != m_editorCommandList.size()) {
        // Now, we must delete any commands after this one.
        m_editorCommandList.erase(m_editorCommandList.begin() + m_editorCommandPtr, m_editorCommandList.end());
    }
    m_editorCommandList.push_back(c);
    m_editorCommandPtr++;
    c->execute();
}

void Editor::render() {
    ZoneScoped;
    auto& app = Application::get();

    // Handle key inputs
    if (Input::key(BLAZAR_KEY_LEFT_CONTROL)) {
        if (Input::key_down(BLAZAR_KEY_Z)) { undo(); }
        if (Input::key_down(BLAZAR_KEY_Y)) { redo(); }
    }

    // Main Menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(app.m_name.c_str())) {
            if (ImGui::MenuItem("Exit")) { app.m_Running = false; }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) { undo(); }
            if (ImGui::MenuItem("Redo", "CTRL+Y")) { redo(); }
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Data")) { ImGui::EndMenu(); }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Logs")) { menu_spawn_unique<LogWindow>(); }
            if (ImGui::MenuItem("Game")) { menu_spawn_unique<GameWindow>(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Input Viewer")) { menu_spawn_unique<InputEditorWindow>(); }
            if (ImGui::MenuItem("Render List Viewer")) { menu_spawn_unique<RenderListViewer>(); }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

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
    for (auto x : m_toAdd) { m_windows.push_back(x); }
    m_toAdd.clear();

    int last_draw_sort = this->editor_get_highest_order();

    if (ImGui::Begin("Editor", nullptr, flags)) {
        auto content_region = ImGui::GetContentRegionMax();

        auto editor_sort = [](Ref<EditorWindow> x, Ref<EditorWindow> y) { return x->m_editorOrder < y->m_editorOrder; };
        std::sort(m_windows.begin(), m_windows.end(), editor_sort);

        int cumulative_width = m_windowPadding;
        int highest_order    = this->editor_get_highest_order();
        for (auto x : m_windows) {
            ImGui::SetCursorPos(ImVec2(cumulative_width, m_windowPadding));

            bool isLast = highest_order == x->m_editorOrder;

            if (x->m_state != EditorWindow::State::FREE_FLOATING) {
                // Determine window width.
                float width = 0.0f;
                switch (x->m_state) {
                    case EditorWindow::State::EDITOR_DOCKED:
                        width = x->m_width_min;
                        break;
                    case EditorWindow::State::DOCKED_EXPANDED:
                        width = x->m_width_expanded;
                        if (isLast && x->m_expandRemainingSpace) {
                            float amount_left = ImGui::GetWindowWidth() - cumulative_width;
                            if (amount_left > width + m_windowPadding) { width = amount_left - m_windowPadding; }
                        }
                        break;
                    case EditorWindow::State::DOCKED_MINIMIZED:
                        width = 32.0f;
                        break;
                    default:
                        width = x->m_width;
                        break;
                }

                x->m_width = width;
                cumulative_width += width + m_innerWindowPadding;
                x->m_height = content_region.y - (2 * m_windowPadding);
            }

            x->draw_editor_window();
        }
    }
    ImGui::End();

    // Now, delete any windows that we were suppose to...
    for (auto x : m_toClose) { m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), x), m_windows.end()); }
    m_toClose.clear();

    if (ImGui::Begin("Editor Command List")) {
        ImGui::Text("Commands:");
        if (ImGui::BeginListBox("##Commands", ImVec2(-1, -1))) {
            if (ImGui::Selectable("Undo all")) {
                while (m_editorCommandPtr > 0) { undo(); }
            }
            for (size_t i = 0; i < this->m_editorCommandList.size(); i++) {
                auto* item = this->m_editorCommandList[i];
                ImGui::PushID(i);
                if (ImGui::Selectable(item->name().c_str(), i == m_editorCommandPtr - 1)) {
                    // We clicked on a newer thing. Fast forward
                    if (i + 1 >= m_editorCommandPtr) {
                        for (size_t j = m_editorCommandPtr; j < i + 1; j++) { redo(); }
                    }
                    // We clicked on an older thing. Rewind
                    if (i + 1 < m_editorCommandPtr) {
                        while (m_editorCommandPtr != (i + 1)) { undo(); }
                    }
                }
                ImGui::PopID();
            }
        }
        ImGui::EndListBox();
    }
    ImGui::End();
}

}  // namespace Editor
}  // namespace Blazar
