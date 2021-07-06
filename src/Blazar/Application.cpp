#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Camera.h"
#include "Blazar/Renderer/OrthographicCamera.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"

#ifdef BLAZAR_IMGUI_ENABLED
#include "Blazar/ImGui/ImGuiLayer.h"
#endif

namespace Blazar {
Application* Application::s_Instance;

Application::Application() {
    BLAZAR_PROFILE_FUNCTION();
    LOG_CORE_TRACE("Creating Application");
    s_Instance = this;
    // Create the window
    m_Window.reset(Window::Create());
    m_Window->SetEventCallback(BLAZAR_BIND_EVENT_FN(Application::OnEvent));
    m_Window->SetVSync(true);

    Renderer::Init(RendererAPI::API::OpenGL);

    m_ImGui = new ImGuiLayer();
    PushOverlay(m_ImGui);

}

Application::~Application() { LOG_CORE_TRACE("Destroying Application"); }

void Application::PushLayer(Layer* layer) {
    BLAZAR_PROFILE_FUNCTION();
    m_LayerStack.PushLayer(layer);
}
void Application::PushOverlay(Layer* layer) {
    BLAZAR_PROFILE_FUNCTION();
    m_LayerStack.PushOverlay(layer);
}

void Application::Run() {
    BLAZAR_PROFILE_FUNCTION();
    while (m_Running) {
        BLAZAR_PROFILE_SCOPE("Main Loop");
        // Start timing
        Timer frameTimer;
        Renderer::NewFrame();

        // Update
        {
            BLAZAR_PROFILE_SCOPE("Update");

            if (Input::KeyPressed(BLAZAR_KEY_GRAVE_ACCENT) && (!m_ImGuiShowKeyPressedLastFrame)) {
                m_RenderImGui = !m_RenderImGui;
            }
            m_ImGuiShowKeyPressedLastFrame = Input::KeyPressed(BLAZAR_KEY_GRAVE_ACCENT);

            for (Layer* layer : m_LayerStack) {
                if (layer->m_UpdatePath == LayerUpdatePath::Update) { layer->OnUpdate(m_deltaTime); }
            }
        }

        // Begin render
        {
            BLAZAR_PROFILE_SCOPE("Render");
            // Clear the screen
            RenderCmd::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
            RenderCmd::Clear();

            // Update all layers
            for (Layer* layer : m_LayerStack) {
                if (layer->m_UpdatePath == LayerUpdatePath::Render) { layer->OnUpdate(m_deltaTime); }
            }

// ImGUI

            m_ImGui->Begin();
            for (Layer* layer : m_LayerStack) { layer->OnImGUIRender(); }
            m_ImGui->End(m_RenderImGui);

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

bool Application::OnWindowClosed(Events::WindowCloseEvent& ev) {
    m_Running = false;
    return true;
}

}  // namespace Blazar
