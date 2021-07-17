#include <bzpch.h>

#include <imgui.h>
#include "CustomImGui.h"

ImFont* imgui_font_normal;
ImFont* imgui_font_big;

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

void CImGUI_Header1(std::string str) { 
    ImGui::PushFont(imgui_font_big);
    ImGui::TextColored(ImVec4(0.7f, 0.80f, 1.00f, 1.0f), "%s", str.c_str());
    ImGui::PopFont();
}
