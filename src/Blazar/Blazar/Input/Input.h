#pragma once

#include <cstdint>
#include <tuple>

namespace Blazar {
typedef int32_t blazar_key_t;

class BLAZAR_API Input {
   public:
    static bool KeyPressed(blazar_key_t key);
    static bool MouseButtonPressed(int mouseButton);
    static std::pair<int, int> GetMousePosition();
    static int GetMouseX();
    static int GetMouseY();

   private:
    // Implementation specific functions, implemented in platform files
    bool PlatformGetKeyPressed(blazar_key_t key) const;
    bool PlatformGetMousePressed(int mouseButton) const;
    std::pair<int, int> PlatformGetMousePosition() const;

   private:
    static Input* s_Instance;
};

};  // namespace Blazar
