#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "TracyOpenGL.hpp"

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
    LOG_CORE_TRACE(" *   Vendor: {0}\n * Renderer: {1}\n *  Version: {2}", glGetString(GL_VENDOR),
                   glGetString(GL_RENDERER), glGetString(GL_VERSION));

    // Check if this is high enough
    int versionMajor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);

    if (versionMajor <= 3) {
        int versionMinor;
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
        if (versionMinor < 5) { BLAZAR_CORE_ASSERT(true, "Blazar requires at least OpenGL 3.5!"); }
    }

    TracyGpuContext;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLContext::Present() {
    glfwSwapBuffers(m_WindowHandle);
    TracyGpuCollect;
}

}  // namespace Blazar
