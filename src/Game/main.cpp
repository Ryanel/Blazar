#define BLAZAR_CREATE_APPLICATION

#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>

#include <Blazar/Input/Keymap.h>
#include <glad/glad.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Debug/DebugLayers.h"

#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Renderer/Shader.h"
#include "Blazar/Renderer/VertexArray.h"

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

        shader.reset(Shader::FromText(vertSrc, fragSrc));
        shader->Bind();
    }

    void OnUpdate() override {
        Renderer::BeginPass();

        shader->Bind();
        Renderer::Submit(sqr_vao);

        shader->Bind();
        Renderer::Submit(tri_vao);

        Renderer::EndPass();
    }

    void OnImGUIRender() override {}

    void OnEvent(Events::Event& ev) {
        
    }

   public:
    std::shared_ptr<VertexArray> tri_vao;
    std::shared_ptr<VertexBuffer> tri_vbo;
    std::shared_ptr<IndexBuffer> tri_ibo;

    std::shared_ptr<VertexArray> sqr_vao;
    std::shared_ptr<VertexBuffer> sqr_vbo;
    std::shared_ptr<IndexBuffer> sqr_ibo;

    std::shared_ptr<Shader> shader;
};

class Game : public Blazar::Application {
   public:
    Game() {
        PushLayer(new LogEventsLayer());
        PushLayer(new ImGUIFPSWindowLayer());
        PushLayer(new ImGUIDemoWindowLayer());
        PushLayer(new ImGUIEditorMainLayer());
        PushLayer(new DebugRenderingLayer());
    }

    ~Game() {}
};

Blazar::Application* Blazar::CreateApplication() { return new Game(); }
