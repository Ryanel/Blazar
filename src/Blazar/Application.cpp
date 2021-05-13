#include "bzpch.h"

#include <glad/glad.h>

#include <Blazar/Application.h>
#include <Blazar/ImGui/ImGuiLayer.h>

namespace Blazar {
Application* Application::s_Instance;

Application::Application() {
    LOG_CORE_TRACE("Creating Application");
    s_Instance = this;

    // Create the window
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BLAZAR_BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() { LOG_CORE_TRACE("Destroying Application"); }

void Application::Run() {
    m_deltaTime = 0.016f;

    ImGuiLayer* imgui = new ImGuiLayer();

    PushOverlay(imgui);

    while (m_Running) {
        // Start timing
        Timer frameTimer;

        // Begin render
        {
            // Clear the screen
            // TODO: Create the renderer
            glClearColor(0.34f, 0.25f, 0.47f, 1.0f);
            glViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
            glClear(GL_COLOR_BUFFER_BIT);

            // Update all layers
            for (Layer* layer : m_LayerStack) { layer->OnUpdate(); }

            // ImGUI
            imgui->Begin();
            for (Layer* layer : m_LayerStack) { layer->OnImGUIRender(); }
            imgui->End();

            // Finish ImGUI
        }

        // Flip Windows
        m_Window->OnUpdate();

        m_deltaTime = frameTimer.Elapsed();
    }
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatch(e);
    dispatch.Dispatch<WindowCloseEvent>(BLAZAR_BIND_EVENT_FN(Application::OnWindowClosed));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.Handled()) { break; }
    }
}
void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
void Application::PushOverlay(Layer* layer) { m_LayerStack.PushOverlay(layer); }

bool Application::OnWindowClosed(WindowCloseEvent& ev) {
    m_Running = false;
    return true;
}
}  // namespace Blazar
