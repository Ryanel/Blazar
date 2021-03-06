#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/Input/Input.h"

// Ordering is important here, do not remove space
#include <GLFW/glfw3.h>

#include "Blazar/Platform/OpenGL/OpenGLContext.h"
#include "Blazar/Platform/Windows/WindowsWindow.h"

namespace Blazar {
static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description) {
    LOG_CORE_ERROR("GLFW Error ({0}: {1}", error, description);
}

Window::~Window() {}
Window* Window::Create(const WindowProperties& props) { return new WindowsWindow(props); }
WindowsWindow::WindowsWindow(const WindowProperties& props) { Init(props); }
WindowsWindow::~WindowsWindow() { Shutdown(); }
bool WindowsWindow::IsVSync() const { return m_Data.VSync; }
void* WindowsWindow::GetNativeWindow() { return (void*)this->m_Window; }
void WindowsWindow::Shutdown() {
    glfwDestroyWindow(m_Window);

    // Delete the context. Not really needed, added to prevent valgrind from complaining.
    // Using delete only because I don't want to incur smart pointer overhead.
    delete m_Context;
}

void WindowsWindow::OnUpdate() {
    ZoneScopedN("Window Flip");
    {
        ZoneScopedN("Poll Events");
        glfwPollEvents();
    }

    {
        ZoneScopedN("Present");
        m_Context->Present();
    }

    m_viewport->width = (float)GetWidth();
    m_viewport->height = (float)GetHeight();
    m_viewport->x = 0.0f;
    m_viewport->y = 0.0f;
}

void WindowsWindow::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

void WindowsWindow::Init(const WindowProperties& props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    m_viewport.reset(new Viewport());
    m_viewport->x = 0.0f;
    m_viewport->y = 0.0f;
    m_viewport->width = (float)props.Width;
    m_viewport->height = (float)props.Height;

    LOG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        BLAZAR_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, props.MSAA);

    {
        ZoneScopedN("GLFW Create Window");
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
    }
    {
        ZoneScopedN("Create OpenGL Context");
        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();
    }

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // GLFW Callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* handle, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(handle);
        data.Width = width;
        data.Height = height;
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* handle) {
        Application::get().m_Running = false;
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
        Input::set_key_state(key, action != GLFW_RELEASE);
    });
}

}  // namespace Blazar
