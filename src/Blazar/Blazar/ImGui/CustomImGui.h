#pragma once
#ifdef BLAZAR_IMGUI_ENABLED
void ImGUI_MenuItem_Toggle_Simple(const char* name, const char* kb, bool& value, bool enabled);
void ImGUI_MainMenu_Toggle_Simple(const char* cat, const char* name, const char* kb, bool& value, bool enabled);
#endif