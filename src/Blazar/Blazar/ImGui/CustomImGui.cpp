#include <bzpch.h>

#include <imgui.h>
#include "CustomImGui.h"

ImFont* imgui_font_normal;
ImFont* imgui_font_big;
ImFont* imgui_font_bigger;

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
    ImGui::PushFont(imgui_font_bigger);
    ImGui::TextColored(EditorUIColor(EditorUICustomColor::Primary), "%s", str.c_str());
    ImGui::PopFont();
}

void CImGUI_Header2(std::string str) {
    ImGui::PushFont(imgui_font_big);
    ImGui::TextColored(EditorUIColor(EditorUICustomColor::Secondary), "%s", str.c_str());
    ImGui::PopFont();
}

void CImGUI_Header2(std::string str, ImVec4 customColor) {
    ImGui::PushFont(imgui_font_big);
    ImGui::TextColored(customColor, "%s", str.c_str());
    ImGui::PopFont();
}

ImVec4 EditorUIColor(EditorUICustomColor color) {
    switch (color) {
        case EditorUICustomColor::Primary:
            return ImColor(77, 179, 255);
        case EditorUICustomColor::Secondary:
            return ImColor(255, 0, 77);
        case EditorUICustomColor::Hilight:
            return ImColor(255, 255, 255);
        case EditorUICustomColor::Alternate:
            return ImColor(255, 128, 230);
        default:
            return ImVec4(1, 1, 1, 1);
    }
}
