#pragma once

#include <cstdint>
#include <tuple>

namespace Blazar {
typedef int32_t blazar_key_t;

/// Handles any mouse, keyboard, or gamepad inputs
class Input {
   public:
    /// Returns true if the key is being held down this frame
    static bool key(blazar_key_t key);
    /// Returns true if the key is pressed this frame
    static bool key_down(blazar_key_t key);
    /// Returns true if the mouse button is being held down this frame
    static bool mouse_down(int mouseButton);
    /// Returns the (x,y) mouse position, relative to the desktop.
    static std::pair<int, int> mouse_pos();
    /// Returns the mouse X position
    static int mouse_x();
    /// Returns the mouse Y position
    static int mouse_y();
    /// Call on a new frame
    static void new_frame();

    /// Called by the platform to set the state of a key
    static void set_key_state(blazar_key_t key, bool pressed);
   private:
    // Implementation specific functions, implemented in platform files
    static bool PlatformGetKeyPressed(blazar_key_t key);
    static bool PlatformGetMousePressed(int mouseButton);
    static std::pair<int, int> PlatformGetMousePosition();


};

};  // namespace Blazar
