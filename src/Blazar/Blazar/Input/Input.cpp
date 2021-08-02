#include "bzpch.h"

#include "Input.h"
#include "Keymap.h"

namespace Blazar {

bool g_input_pressed[BLAZAR_KEYMAP_SIZE];
bool g_input_pressed_last[BLAZAR_KEYMAP_SIZE];

bool Input::key(blazar_key_t key) { return g_input_pressed[key - BLAZAR_KEY_MIN]; }

bool Input::key_down(blazar_key_t key) {
    blazar_key_t index = key - BLAZAR_KEY_MIN;
    if (g_input_pressed[index]) { return !g_input_pressed_last[index]; }
    return false;
}
bool Input::mouse_down(int mouseButton) { return Input::PlatformGetMousePressed(mouseButton); }
std::pair<int, int> Input::mouse_pos() { return Input::PlatformGetMousePosition(); }
int Input::mouse_x() { return mouse_pos().first; }
int Input::mouse_y() { return mouse_pos().second; }
void Input::new_frame() {
    ZoneScoped;
    // Preserve last frame's inputs
    memcpy(&g_input_pressed_last, &g_input_pressed, sizeof(bool) * BLAZAR_KEYMAP_SIZE);
}

void Input::set_key_state(blazar_key_t key, bool pressed) { g_input_pressed[key - BLAZAR_KEY_MIN] = pressed; }

}  // namespace Blazar
