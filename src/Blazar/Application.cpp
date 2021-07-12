#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"

#include "Blazar/ImGui/ImGuiLayer.h"

#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Tracy.hpp"
#include "glad/glad.h"

namespace Blazar {
Application* Application::s_Instance;

Application::Application() {
    LOG_CORE_TRACE("Creating Application");
    s_Instance = this;
    // Create the window
    m_Window.reset(Window::Create());
    m_Window->SetEventCallback(BLAZAR_BIND_EVENT_FN(Application::OnEvent));
    m_Window->SetVSync(true);

    Renderer::Init(RendererAPI::API::OpenGL);

    m_ImGui = new ImGuiLayer();
    m_RenderViewport = std::make_shared<Viewport>(0, 0, 32, 32);

    RenderTextureProperties renderProperties;
    renderProperties.width = m_RenderViewport->width;
    renderProperties.height = m_RenderViewport->height;
    renderProperties.msaa = 1;

    m_GameRenderTexture = RenderTexture::Create(renderProperties);

    PushOverlay(m_ImGui);
}

Application::~Application() { LOG_CORE_TRACE("Destroying Application"); }

void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
void Application::PushOverlay(Layer* layer) { m_LayerStack.PushOverlay(layer); }

void Application::Run() {
    ZoneScoped;

    // Create a fullscreen quad to render the game to.
    BufferLayout quad_layout = {
        {ShaderDataType::Float3, "a_Position"},  // 00: Position
        {ShaderDataType::Float2, "a_TexCoord"},  // 12: Color
    };

    float quad_verts[5 * 4] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // v0
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,  // v1
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  // v2,
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // v3,
    };

    uint32_t quad_indicies[6] = {0, 1, 2, 2, 3, 0};
    Ref<VertexBuffer> quad_vbo = VertexBuffer::Create(quad_verts, sizeof(quad_verts), quad_layout);
    Ref<IndexBuffer> quad_ibo = IndexBuffer::Create(quad_indicies, sizeof(quad_indicies));
    Ref<VertexArray> quad_vao = VertexArray::Create(quad_vbo, quad_ibo);

    // Setup the shader
    Ref<Shader> fullscreenShader = Shader::FromFile("Contents/Shaders/ScreenTexture");
    fullscreenShader->Bind();
    std::dynamic_pointer_cast<Blazar::OpenGLShader>(fullscreenShader)->SetInt("u_Texture", 0);

    // Run Loop
    while (m_Running) {
        Timer frameTimer;

        // Check if the renderbuffer needs to be changed.
        if (!(m_UseEditorWindow && m_RenderImGui)) {
            m_RenderViewport->Set(0, 0, GetWindow().GetViewport()->width, GetWindow().GetViewport()->height);
        }
        RenderTextureProperties renderProperties(m_RenderViewport->width, m_RenderViewport->height);

        if ((m_GameRenderTexture->GetWidth() != renderProperties.width) ||
            (m_GameRenderTexture->GetHeight() != renderProperties.height)) {
            m_GameRenderTexture = RenderTexture::Create(renderProperties);
            m_GameRenderTexture->Unbind();
        }

        // Update Code. No rendering here
        {
            ZoneScopedN("Update");

            if (Input::KeyPressed(BLAZAR_KEY_GRAVE_ACCENT) && (!m_ImGuiShowKeyPressedLastFrame)) {
                m_RenderImGui = !m_RenderImGui;
            }
            m_ImGuiShowKeyPressedLastFrame = Input::KeyPressed(BLAZAR_KEY_GRAVE_ACCENT);

            for (Layer* layer : m_LayerStack) {
                if (layer->m_UpdatePath == LayerUpdatePath::Update) { layer->OnUpdate(m_deltaTime); }
            }
        }

        // Rendering Code!
        {
            ZoneScopedN("Render");
            renderer_stats.passesThisFrame = 0;
            renderer_stats.drawCalls = 0;
            // Clear the screen
            RenderCmd::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
            RenderCmd::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            RenderCmd::Clear();

            // Render Game
            {
                RenderCmd::SetRenderTexture(m_GameRenderTexture);
                RenderCmd::SetViewport(0, 0, m_GameRenderTexture->GetWidth(), m_GameRenderTexture->GetHeight());
                RenderCmd::SetClearColor(0.05f, 0.05f, 0.1f, 1.0f);
                RenderCmd::Clear();

                // Update all layers
                for (Layer* layer : m_LayerStack) {
                    if (layer->m_UpdatePath == LayerUpdatePath::Render) { layer->OnUpdate(m_deltaTime); }
                }

                RenderCmd::SetRenderTexture(nullptr);

                if (!m_RenderImGui || !m_UseEditorWindow && false) {
                    RenderCmd::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
                    RenderCmd::SetShader(fullscreenShader);
                    RenderCmd::BindTexture(m_GameRenderTexture->m_ColorTexture);
                    RenderCmd::DrawIndexed(quad_vao);
                }
            }

            Renderer::Submit(new RenderItem(RenderItemType::FRAME_SYNC));
            Renderer::ProcessQueue();

            // ImGUI
            m_ImGui->Begin();
            for (Layer* layer : m_LayerStack) { layer->OnImGUIRender(); }
            m_ImGui->End(m_RenderImGui);

            Renderer::Flush();
        }

        // Flip Windows
        {
            ZoneScopedN("Window Flip");
            m_Window->OnUpdate();
        }

        FrameMark;
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
