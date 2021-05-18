#include "bzpch.h"

#include <glad/glad.h>

#include "Blazar/Application.h"
#include "Blazar/ImGui/ImGuiLayer.h"

// Sample
#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Shader.h"
#include "Blazar/Renderer/VertexArray.h"

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

    Renderer::Init(RendererAPI::API::OpenGL);

    // Setup Global Layers
    ImGuiLayer* imgui = new ImGuiLayer();
    PushOverlay(imgui);

    // Sample rendering code
    float tri_verts[7 * 3] = {
        -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,  // v0
        0.5f,  -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0,  // v1
        0.0f,  0.5f,  0.0f, 0.0, 0.0, 1.0, 1.0,  // v2,
    };

    uint32_t tri_indicies[3] = {0, 1, 2};

    BufferLayout tri_layout = {
        {ShaderDataType::Float3, "a_Position"},  // 00: Position
        {ShaderDataType::Float4, "a_Color"},     // 12: Color
    };

    std::shared_ptr<VertexArray> tri_vao;
    std::shared_ptr<VertexBuffer> tri_vbo;
    std::shared_ptr<IndexBuffer> tri_ibo;

    tri_vao.reset(VertexArray::Create());
    tri_vbo.reset(VertexBuffer::Create(tri_verts, sizeof(tri_verts)));
    tri_ibo.reset(IndexBuffer::Create(tri_indicies, sizeof(tri_indicies)));

    tri_vbo->SetLayout(tri_layout);

    tri_vao->Bind();
    tri_vao->AddVertexBuffer(tri_vbo);
    tri_vao->SetIndexBuffer(tri_ibo);

    tri_vao->Unbind();

    float sqr_verts[7 * 4] = {
        -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,  // v0
        0.5f,  -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0,  // v1
        0.5f,  0.5f,  0.0f, 0.0, 0.0, 1.0, 1.0,  // v2,
        -0.5f, 0.5f,  0.0f, 1.0, 0.0, 1.0, 1.0,  // v3,
    };

    uint32_t sqr_indicies[6] = {0, 1, 2, 2, 3, 0};

    BufferLayout sqr_layout = {
        {ShaderDataType::Float3, "a_Position"},  // 00: Position
        {ShaderDataType::Float4, "a_Color"},     // 12: Color
    };

    std::shared_ptr<VertexArray> sqr_vao;
    std::shared_ptr<VertexBuffer> sqr_vbo;
    std::shared_ptr<IndexBuffer> sqr_ibo;

    sqr_vao.reset(VertexArray::Create());
    sqr_vbo.reset(VertexBuffer::Create(sqr_verts, sizeof(sqr_verts)));
    sqr_ibo.reset(IndexBuffer::Create(sqr_indicies, sizeof(sqr_indicies)));

    sqr_vbo->SetLayout(sqr_layout);

    sqr_vao->Bind();
    sqr_vao->AddVertexBuffer(sqr_vbo);
    sqr_vao->SetIndexBuffer(sqr_ibo);

    std::string vertSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        out vec4 v_Color;
        
        void main()
        {
            v_Color = a_Color;
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string fragSrc = R"(
        #version 330 core
        layout(location = 0) out vec4 color;

        in vec4 v_Color;
        void main()
        {
           color = vec4(1 - v_Color.rgb, 1);
        }
    )";

    Shader s = Shader::FromText(vertSrc, fragSrc);

    s.Bind();

    while (m_Running) {
        // Start timing
        Timer frameTimer;

        // Begin render
        {
            // Clear the screen
            // TODO: Create the renderer
            glViewport(0, 0, GetWindow().GetWidth(), GetWindow().GetHeight());
            RenderCmd::Clear();

            Renderer::BeginPass();

            s.Bind();
            Renderer::Submit(sqr_vao);

            s.Bind();
            Renderer::Submit(tri_vao);

            Renderer::EndPass();

            //Renderer::Flush();
            
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

void Application::OnEvent(Events::Event& e) {
    Events::EventDispatcher dispatch(e);
    dispatch.Dispatch<Events::WindowCloseEvent>(BLAZAR_BIND_EVENT_FN(Application::OnWindowClosed));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.Handled()) { break; }
    }
}

void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
void Application::PushOverlay(Layer* layer) { m_LayerStack.PushOverlay(layer); }

bool Application::OnWindowClosed(Events::WindowCloseEvent& ev) {
    m_Running = false;
    return true;
}

}  // namespace Blazar
