#include <bzpch.h>

#ifdef BLAZAR_IMGUI_ENABLED
#include <imgui.h>

#include "CustomImGui.h"

void ImGUI_MenuItem_Toggle_Simple(const char* name, const char* kb, bool& value, bool enabled = true) {
    if (ImGui::MenuItem(name, kb, value, enabled)) { value = !value; }
}

void ImGUI_MainMenu_Toggle_Simple(const char* cat, const char* name, const char* kb, bool& value, bool enabled) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(cat)) {
            ImGUI_MenuItem_Toggle_Simple(name, kb, value, enabled);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
#endif