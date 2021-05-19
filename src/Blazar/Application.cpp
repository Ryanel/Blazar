#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/ImGui/ImGuiLayer.h"

#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Renderer/RenderCmd.h"

namespace Blazar {
Application* Application::s_Instance;

Application::Application() {
    LOG_CORE_TRACE("Creating Application");
    s_Instance = this;

    // Create the window
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BLAZAR_BIND_EVENT_FN(Application::OnEvent));
    Renderer::Init(RendererAPI::API::OpenGL);

    m_ImGui = new ImGuiLayer();
    PushOverlay(m_ImGui);
}

Application::~Application() { LOG_CORE_TRACE("Destroying Application"); }

void Application::Run() {
    m_deltaTime = 0.016f;

    while (m_Running) {
        // Start timing
        Timer frameTimer;

        // Begin render
        {
            // Clear the screen
            RenderCmd::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
            RenderCmd::Clear();

            // Update all layers
            for (Layer* layer : m_LayerStack) { layer->OnUpdate(); }

            // ImGUI
            m_ImGui->Begin();
            for (Layer* layer : m_LayerStack) { layer->OnImGUIRender(); }
            m_ImGui->End();

        }

        // Flip Windows
        m_Window->OnUpdate();

        m_deltaTime = frameTimer.Elapsed();
    }
}

void Application::OnEvent(Events::Event& e) {
    Events::EventDispatcher dispatch(e);
    dispatch.Dispatch<Events::WindowCloseEvent>(BLAZAR_BIND_EVENT_FN(Application::OnWindowClosed));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.Handled()) { break; }
    }
}

void Application::PushLayer(Layer* layer) { 
    m_LayerStack.PushLayer(layer); 

}
void Application::PushOverlay(Layer* layer) {
    m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowClosed(Events::WindowCloseEvent& ev) {
    m_Running = false;
    return true;
}

}  // namespace Blazar
