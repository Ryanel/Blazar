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
    float vertices[3 * 3] = {
        -0.5f, -0.5f, 0.0f,  // v0
        0.5f,  -0.5f, 0.0f,  // v1
        0.0f,  0.5f,  0.0f,  // v2
    };

    uint32_t indicies[3] = {0, 1, 2};

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexBuffer* buffer = VertexBuffer::Create(vertices, sizeof(vertices));
    buffer->Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    IndexBuffer* indexBuffer = IndexBuffer::Create(indicies, sizeof(indicies));
    indexBuffer->Bind();

    std::string vertSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 a_Position;

        out vec3 v_Position;

        void main()
        {
            v_Position = a_Position;
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string fragSrc = R"(
        #version 330 core
        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        void main()
        {
           color = vec4(v_Position * 0.5 + 0.5, (1 / 1.5)) * 1.5;
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
