#include "bzpch.h"

#include "Blazar/Input/Input.h"

namespace Blazar {

bool Input::KeyPressed(blazar_key_t key) { return s_Instance->PlatformGetKeyPressed(key); }
bool Input::MouseButtonPressed(int mouseButton) { return s_Instance->PlatformGetMousePressed(mouseButton); }
std::pair<int, int> Input::GetMousePosition() { return s_Instance->PlatformGetMousePosition(); }
int Input::GetMouseX() { return GetMousePosition().first; }
int Input::GetMouseY() { return GetMousePosition().second; }

}  // namespace Blazar
