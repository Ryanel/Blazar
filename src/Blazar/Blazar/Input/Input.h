#pragma once

#include <cstdint>
#include <tuple>

namespace Blazar {
typedef int32_t blazar_key_t;

class Input {
   public:
    /// Returns true if the key is being held down this frame
    static bool Key(blazar_key_t key);
    /// Returns true if the key is pressed this frame
    static bool KeyDown(blazar_key_t key);
    /// Returns true if the mouse button is being held down this frame
    static bool MouseButton(int mouseButton);
    /// Returns the (x,y) mouse position, relative to the desktop.
    static std::pair<int, int> MousePosition();
    // Returns the mouse X position
    static int GetMouseX();
    /// Returns the mouse Y position
    static int GetMouseY();
    /// 
    static void NewFrame();

    // Called by the platform to set the state of a key
    static void SetKeyState(blazar_key_t key, bool pressed);
   private:
    // Implementation specific functions, implemented in platform files
    static bool PlatformGetKeyPressed(blazar_key_t key);
    static bool PlatformGetMousePressed(int mouseButton);
    static std::pair<int, int> PlatformGetMousePosition();


};

};  // namespace Blazar
