#include "bzpch.h"

#include <GLFW/glfw3.h>
#include <Blazar/Input/Input.h>
#include <Blazar/Application.h>

namespace Blazar {

Input* Input::s_Instance = new Input();

bool Input::PlatformGetKeyPressed(blazar_key_t key) const {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::PlatformGetMousePressed(int mouseNumber) const {
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, mouseNumber);
    return state == GLFW_PRESS;
}
std::pair<int, int> Input::PlatformGetMousePosition() const { 
    double x, y;

    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    glfwGetCursorPos(window, &x, &y);

    return std::pair<int, int>((int)x,(int)y); 
}

}  // namespace Blazar