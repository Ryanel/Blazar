#define BLAZAR_CREATE_APPLICATION

#include <glad/glad.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Debug/DebugLayers.h"

#include "Blazar/Blazar.h"
#include "Blazar/Events/AppEvents.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/Camera.h"
#include "Blazar/Renderer/OrthographicCamera.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Renderer/Shader.h"
#include "Blazar/Renderer/VertexArray.h"
#include "Blazar/Renderer/Viewport.h"

using namespace Blazar;

class ImGUIEditorMainLayer : public Blazar::Layer {
   public:
    ImGUIEditorMainLayer() : Layer("ImGUI Editor Main") {}

    void OnImGUIRender() override {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) { Application::Get().m_Running = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
};

class DebugRenderingLayer : public Blazar::Layer {
   public:
    DebugRenderingLayer() : Layer("Game") {}

    void OnAttach() override {
        // Triangle
        BufferLayout tri_layout = {
            {ShaderDataType::Float3, "a_Position"},  // 00: Position
            {ShaderDataType::Float4, "a_Color"},     // 12: Color
        };

        float tri_verts[7 * 3] = {
            -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,  // v0
            0.5f,  -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0,  // v1
            0.0f,  0.5f,  0.0f, 0.0, 0.0, 1.0, 1.0,  // v2,
        };

        uint32_t tri_indicies[3] = {0, 1, 2};

        tri_vao.reset(VertexArray::Create());
        tri_vbo.reset(VertexBuffer::Create(tri_verts, sizeof(tri_verts)));
        tri_ibo.reset(IndexBuffer::Create(tri_indicies, sizeof(tri_indicies)));

        tri_vbo->SetLayout(tri_layout);

        tri_vao->Bind();
        tri_vao->AddVertexBuffer(tri_vbo);
        tri_vao->SetIndexBuffer(tri_ibo);

        tri_vao->Unbind();

        // Square
        BufferLayout sqr_layout = {
            {ShaderDataType::Float3, "a_Position"},  // 00: Position
            {ShaderDataType::Float4, "a_Color"},     // 12: Color
        };

        float sqr_verts[7 * 4] = {
            -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,  // v0
            0.5f,  -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0,  // v1
            0.5f,  0.5f,  0.0f, 0.0, 0.0, 1.0, 1.0,  // v2,
            -0.5f, 0.5f,  0.0f, 1.0, 0.0, 1.0, 1.0,  // v3,
        };

        uint32_t sqr_indicies[6] = {0, 1, 2, 2, 3, 0};

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
            
            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec4 v_Color;
        
            void main()
            {
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

        shader.reset(Shader::FromText(vertSrc, fragSrc));
        shader->Bind();

        // Camera
        float aspect = Application::Get().GetWindow().GetAspect();
        cam.reset(new OrthographicCamera(m_Zoom, aspect));
        cam->SetPosition({0, 0, 0});
    }

    void OnUpdate() override {
        // Input
        float delta = Application::Get().m_deltaTime;
        auto windowViewport = Application::Get().GetWindow().GetViewport();

        if (Input::KeyPressed(BLAZAR_KEY_W)) { m_CameraPosition.y += camSpeed * delta; }
        if (Input::KeyPressed(BLAZAR_KEY_A)) { m_CameraPosition.x -= camSpeed * delta; }
        if (Input::KeyPressed(BLAZAR_KEY_D)) { m_CameraPosition.x += camSpeed * delta; }
        if (Input::KeyPressed(BLAZAR_KEY_S)) { m_CameraPosition.y -= camSpeed * delta; }

        cam->SetViewport(windowViewport);
        cam->SetZoom(m_Zoom);
        cam->SetPosition(m_CameraPosition);

        glm::mat4 tri_pos = glm::translate(glm::mat4(1.0f), {0.5f, 0, 0});
        glm::mat4 sqr_pos = glm::translate(glm::mat4(1.0f), {-0.5f, 0, 0});

        Renderer::BeginPass(*cam);

        Renderer::Submit(sqr_vao, shader, sqr_pos);
        Renderer::Submit(tri_vao, shader, tri_pos);

        Renderer::EndPass();
    }

    void OnImGUIRender() override {
        if (ImGui::Begin("Camera Controls")) {
            float realWidth = m_Zoom * 2;
            float realHeight = m_Zoom * 2;

            ImGui::SliderFloat("Camera Speed", &camSpeed, 0.01f, 4);
            ImGui::SliderFloat("Zoom", &m_Zoom, 1, 10);
            ImGui::DragFloat3("Position", &m_CameraPosition.x, 0.025f);

            ImGui::Text("Real Width: %f", realWidth);
            ImGui::Text("Real Height: %f", realHeight);
        }
        ImGui::End();
    }

    void OnEvent(Events::Event& ev) {}

   public:
    std::shared_ptr<VertexArray> tri_vao;
    std::shared_ptr<VertexBuffer> tri_vbo;
    std::shared_ptr<IndexBuffer> tri_ibo;

    std::shared_ptr<VertexArray> sqr_vao;
    std::shared_ptr<VertexBuffer> sqr_vbo;
    std::shared_ptr<IndexBuffer> sqr_ibo;

    std::shared_ptr<Shader> shader;

    std::shared_ptr<OrthographicCamera> cam;

    float m_Zoom = 3.0;
    glm::vec3 m_CameraPosition = {0, 0, 0};
    float camSpeed = 1.0f;
};

class Game : public Blazar::Application {
   public:
    Game() {
        // PushLayer(new LogEventsLayer());
        PushLayer(new ImGUIFPSWindowLayer());
        PushLayer(new ImGUIDemoWindowLayer());
        PushLayer(new ImGUIEditorMainLayer());
        PushLayer(new DebugRenderingLayer());
    }

    ~Game() {}
};

Blazar::Application* Blazar::CreateApplication() { return new Game(); }
