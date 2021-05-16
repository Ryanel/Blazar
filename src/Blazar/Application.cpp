#include "bzpch.h"

#include <glad/glad.h>

#include "Blazar/Application.h"
#include "Blazar/ImGui/ImGuiLayer.h"

// Sample
#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Renderer/Shader.h"

namespace Blazar {
Application* Application::s_Instance;

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
    }
}

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

    // Sample rendering code
    float vertices[10 * 3] = {
        -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,  // v0
        0.5f,  -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0,  // v1
        0.0f,  0.5f,  0.0f, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0,  // v2
    };

    uint32_t indicies[3] = {0, 1, 2};

    BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},  // 00: Position
        {ShaderDataType::Float4, "a_Color"},     // 12: Color
        {ShaderDataType::Float3, "a_Normal"}     // 28: Normal
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexBuffer* buffer = VertexBuffer::Create(vertices, sizeof(vertices));
    buffer->Bind();

    uint32_t index = 0;

    for (const auto& element : layout.GetElements()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetElementCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void*)element.Offset);
        index++;
    }

    IndexBuffer* indexBuffer = IndexBuffer::Create(indicies, sizeof(indicies));
    indexBuffer->Bind();

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
           color = v_Color;
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
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

    delete buffer;
    delete indexBuffer;
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
