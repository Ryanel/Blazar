#pragma once

#include <string>
#include <imgui.h>

extern ImFont* imgui_font_normal;
extern ImFont* imgui_font_big;

void ImGUI_MenuItem_Toggle_Simple(const char* name, const char* kb, bool& value, bool enabled);
void ImGUI_MainMenu_Toggle_Simple(const char* cat, const char* name, const char* kb, bool& value, bool enabled);

void CImGUI_Header1(std::string str);