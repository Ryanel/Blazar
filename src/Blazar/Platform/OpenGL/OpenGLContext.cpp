#include "bzpch.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace Blazar {

OpenGLContext::OpenGLContext(GLFWwindow* handle) : m_WindowHandle(handle) {
    BLAZAR_CORE_ASSERT(m_WindowHandle != nullptr, "Null Window Handle Passed to OpenGL Context!");
    LOG_CORE_INFO("Created new OpenGL Rendering Context");
}

void OpenGLContext::Init() {
    // Make active window in glfw
    glfwMakeContextCurrent(m_WindowHandle);

    // Init GLAD for Window
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    BLAZAR_CORE_ASSERT(status, "Could not initialize GLAD!");

    // Log renderer information.
    LOG_CORE_INFO(" *   Vendor: {0}", glGetString(GL_VENDOR));
    LOG_CORE_INFO(" * Renderer: {0}", glGetString(GL_RENDERER));
    LOG_CORE_INFO(" *  Version: {0}", glGetString(GL_VERSION));
    glEnable(GL_MULTISAMPLE);
}

void OpenGLContext::Present() { glfwSwapBuffers(m_WindowHandle); }

}  // namespace Blazar
