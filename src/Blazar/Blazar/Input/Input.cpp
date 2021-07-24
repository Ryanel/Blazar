#include "bzpch.h"

#include "Input.h"
#include "Keymap.h"

namespace Blazar {

bool g_input_pressed[BLAZAR_KEYMAP_SIZE];
bool g_input_pressed_last[BLAZAR_KEYMAP_SIZE];

bool Input::Key(blazar_key_t key) { return g_input_pressed[key - BLAZAR_KEY_MIN]; }

bool Input::KeyDown(blazar_key_t key) {
    blazar_key_t index = key - BLAZAR_KEY_MIN;
    if (g_input_pressed[index]) { return !g_input_pressed_last[index]; }
    return false;
}
bool Input::MouseButton(int mouseButton) { return Input::PlatformGetMousePressed(mouseButton); }
std::pair<int, int> Input::MousePosition() { return Input::PlatformGetMousePosition(); }
int Input::GetMouseX() { return MousePosition().first; }
int Input::GetMouseY() { return MousePosition().second; }
void Input::NewFrame() {
    ZoneScoped;
    // Preserve last frame's inputs
    memcpy(&g_input_pressed_last, &g_input_pressed, sizeof(bool) * BLAZAR_KEYMAP_SIZE);
}

void Input::SetKeyState(blazar_key_t key, bool pressed) { g_input_pressed[key - BLAZAR_KEY_MIN] = pressed; }

}  // namespace Blazar
