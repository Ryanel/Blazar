#include "bzpch.h"

#include <Platform/OpenGL/OpenGLContext.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace Blazar {

OpenGLContext::OpenGLContext(GLFWwindow* handle) : m_WindowHandle(handle) {
    BLAZAR_CORE_ASSERT(handle != nullptr, "Null Window Handle Passed to OpenGL Context!");
    LOG_CORE_INFO("Created new OpenGL Rendering Context");
}

void OpenGLContext::Init() {
    // Init GLAD for Window
    glfwMakeContextCurrent(m_WindowHandle);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    BLAZAR_CORE_ASSERT(status, "Could not initialize GLAD!");
}

void OpenGLContext::Present() { glfwSwapBuffers(m_WindowHandle); }

}  // namespace Blazar
