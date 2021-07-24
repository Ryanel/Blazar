#include "bzpch.h"

#include "Blazar/Application.h"
#include "Blazar/ImGui/ImGuiLayer.h"
#include "Blazar/Input/Input.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Primitives/Buffer.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Time/Timer.h"
#include "Blazar/Window.h"

#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Tracy.hpp"

namespace Blazar {
Application* Application::s_Instance;

Application::Application() {
    LOG_CORE_TRACE("Creating Application");
    s_Instance = this;
    m_Window.reset(Window::Create());
    m_Window->SetVSync(true);

    Renderer::Init(RendererAPI::API::OpenGL);

    m_ImGui = new ImGuiLayer();
    m_RenderViewport = std::make_shared<Viewport>(0, 0, 32, 32);

    RenderTextureProperties renderProperties;
    renderProperties.width =(int)m_RenderViewport->width;
    renderProperties.height = (int)m_RenderViewport->height;
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
    Ref<Shader> fullscreenShader = Shader::FromFile("Contents/Data/Shaders/ScreenTexture");
    fullscreenShader->SetName("ScreenTexture");
    fullscreenShader->Bind();
    std::dynamic_pointer_cast<Blazar::OpenGLShader>(fullscreenShader)->SetInt("u_Texture", 0);

    // Run Loop
    while (m_Running) {
        ZoneScopedN("Run loop");
        Timer frameTimer;

        // Update Code. No rendering here
        {
            ZoneScopedN("Update");

            if (Input::KeyDown(BLAZAR_KEY_GRAVE_ACCENT)) { m_RenderImGui = !m_RenderImGui; }
            {
                ZoneScopedN("Layer Update");
                for (Layer* layer : m_LayerStack) {
                    if (((int)layer->m_UpdatePath & (int)LayerUpdatePath::Update) != 0) {
                        layer->OnUpdate(m_deltaTime);
                    }
                }
            }

            Input::NewFrame();
        }

        // Rendering Code
        {
            ZoneScopedN("Render");
            {
                ZoneScopedN("Resize Render Texture");
                // Check if the renderbuffer needs to be changed.
                if (!(m_UseEditorWindow && m_RenderImGui)) {
                    m_RenderViewport->Set(0, 0, GetWindow().GetViewport()->width, GetWindow().GetViewport()->height);
                }

                RenderTextureProperties renderProperties((int)m_RenderViewport->width, (int)m_RenderViewport->height);

                if ((m_GameRenderTexture->GetWidth() != renderProperties.width) ||
                    (m_GameRenderTexture->GetHeight() != renderProperties.height)) {
                    m_GameRenderTexture = RenderTexture::Create(renderProperties);
                    m_GameRenderTexture->Unbind();
                }
            }

            // Render Game
            {
                ZoneScopedN("Render Command");
                // Clear the screen
                Renderer::ResetStats();
                RenderCmd::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
                RenderCmd::Clear(0.3f, 0.3f, 0.3f, 1.0f);

                RenderCmd::SetRenderTexture(m_GameRenderTexture);
                RenderCmd::SetViewport(0, 0, m_GameRenderTexture->GetWidth(), m_GameRenderTexture->GetHeight());
                RenderCmd::Clear(0.05f, 0.1f, 0.2f, 1.0f);

                // Update all layers
                {
                    ZoneScopedN("Layer Render");
                    for (Layer* layer : m_LayerStack) {
                        if (((int)layer->m_UpdatePath & (int)LayerUpdatePath::Render) != 0) {
                            layer->OnRender(m_deltaTime);
                        }
                    }
                }

                RenderCmd::SetRenderTexture(nullptr);

                if (!m_RenderImGui || !m_UseEditorWindow && false) {
                    ZoneScopedN("Render Texture to Quad");
                    RenderCmd::SetViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
                    RenderCmd::SetShader(fullscreenShader);
                    RenderCmd::BindTexture(m_GameRenderTexture->m_ColorTexture);
                    RenderCmd::DrawIndexed(quad_vao);
                }
                Renderer::Submit(RenderCommand(RenderCommandID::FRAME_SYNC));
            }

            Renderer::FlushQueue();

            // ImGUI
            {
                ZoneScopedN("ImGUI");
                m_ImGui->Begin();
                for (Layer* layer : m_LayerStack) { layer->OnImGUIRender(); }
                m_ImGui->End(m_RenderImGui);
            }
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

}  // namespace Blazar
