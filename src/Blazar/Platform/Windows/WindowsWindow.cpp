#include "bzpch.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Blazar/Events/AppEvents.h"
#include "Blazar/Events/KeyEvents.h"
#include "Blazar/Events/MouseEvents.h"

// Ordering is important here, do not remove space
#include <GLFW/glfw3.h>
#include "Platform/OpenGL/OpenGLContext.h"

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
void WindowsWindow::Shutdown() { glfwDestroyWindow(m_Window); }

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    m_Context->Present();
}

void WindowsWindow::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.VSync = enabled;
}

void WindowsWindow::Init(const WindowProperties& props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    LOG_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        BLAZAR_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_SAMPLES, 16);

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // GLFW Callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* handle, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(handle);

        data.Width = width;
        data.Height = height;

        WindowResizeEvent ev(width, height);
        data.EventCallback(ev);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* handle) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(handle);

        WindowCloseEvent ev;
        data.EventCallback(ev);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* handle, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(handle);

        switch (action) {
            case GLFW_PRESS: {
                KeyDownEvent ev(key, false);
                data.EventCallback(ev);
                break;
            }

            case GLFW_RELEASE: {
                KeyUpEvent ev(key);
                data.EventCallback(ev);
                break;
            }

            case GLFW_REPEAT: {
                KeyDownEvent ev(key, true);
                data.EventCallback(ev);
                break;
            }
            default:
                break;
        }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* handle, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(handle);

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent ev(button);
                data.EventCallback(ev);
                break;
            }

            case GLFW_RELEASE: {
                MouseButtonReleasedEvent ev(button);
                data.EventCallback(ev);
                break;
            }

            default:
                break;
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* handle, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(handle);

        MouseScrolledEvent ev(xOffset, yOffset);
        data.EventCallback(ev);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* handle, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(handle);

        MouseMovedEvent ev(xPos, yPos);
        data.EventCallback(ev);
    });
}

}  // namespace Blazar
