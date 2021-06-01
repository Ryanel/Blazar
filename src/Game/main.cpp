#define BLAZAR_CREATE_APPLICATION

#include <glad/glad.h>
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Debug/DebugLayers.h"

#include "Blazar/Blazar.h"
#include "Blazar/Entry.h"
#include "Blazar/Events/AppEvents.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/ImGui/ImGuiLog.h"
#include "Blazar/Input/Keymap.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/Camera.h"
#include "Blazar/Renderer/OrthographicCamera.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Renderer/Shader.h"
#include "Blazar/Renderer/Texture.h"
#include "Blazar/Renderer/VertexArray.h"
#include "Blazar/Renderer/Viewport.h"

using namespace Blazar;

#ifdef BLAZAR_IMGUI_ENABLED
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

        ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
    }
};
#endif

class DebugRenderingLayer : public Blazar::Layer {
   public:
    DebugRenderingLayer() : Layer("Game") {}

    void OnAttach() override {
        // Square
        BufferLayout sqr_layout = {
            {ShaderDataType::Float3, "a_Position"},  // 00: Position
            {ShaderDataType::Float2, "a_TexCoord"},  // 12: Color
        };

        float sqr_verts[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // v0
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // v1
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // v2,
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  // v3,
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
            layout(location = 1) in vec2 a_TexCoord;
            
            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;
            void main()
            {
                v_TexCoord = a_TexCoord;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform sampler2D u_Texture;
            
            void main()
            {
               color = texture(u_Texture, v_TexCoord);
            }
        )";
 
        shader = Shader::FromText(vertSrc, fragSrc);
        shader->Bind();
        std::dynamic_pointer_cast<Blazar::OpenGLShader>(shader)->SetInt("u_Texture", 0);
        sampleTexture = Texture2D::Create("Contents/Textures/SampleTrans.png");

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
        if (Input::KeyPressed(BLAZAR_KEY_Q)) { LOG_CORE_TRACE("Test Log"); }

        cam->SetViewport(windowViewport);
        cam->SetZoom(m_Zoom);
        cam->SetPosition(m_CameraPosition);

        glm::mat4 sqr_pos = glm::translate(glm::mat4(1.0f), {0, 0, 0});

        Renderer::BeginPass(*cam);

        sampleTexture->Bind(0);
        Renderer::Submit(sqr_vao, shader, sqr_pos);

        Renderer::EndPass();
    } 

#ifdef BLAZAR_IMGUI_ENABLED
    void OnImGUIRender() override {
        ImGUI_MainMenu_Toggle_Simple("Windows", "Camera Controls", "", this->showCameraControls, true);

        if (!this->showCameraControls) { return; }

        if (ImGui::Begin("Camera Controls", &this->showCameraControls)) {
            float realWidth = m_Zoom * 2;
            float realHeight = m_Zoom * 2;

            ImGui::SliderFloat("Camera Speed", &camSpeed, 0.01f, 4);
            ImGui::SliderFloat("Zoom", &m_Zoom, 1, 10);
            ImGui::DragFloat3("Position", &m_CameraPosition.x, 0.025f);

            ImGui::Text("Real Width: %f, Height: %f", realWidth, realHeight);
        }
        ImGui::End();
    }
#endif

    void OnEvent(Events::Event& ev) {}

   public:
    Ref<VertexArray> sqr_vao;
    Ref<VertexBuffer> sqr_vbo;
    Ref<IndexBuffer> sqr_ibo;

    Ref<Shader> shader;
    Ref<Texture> sampleTexture;
    Ref<OrthographicCamera> cam;

    float m_Zoom = 3.0;
    glm::vec3 m_CameraPosition = {0, 0, 0};
    float camSpeed = 1.0f;
    bool showCameraControls = true;
    ;
};

class Game : public Blazar::Application {
   public:
    Game() {
        // PushLayer(new LogEventsLayer());
        #ifdef BLAZAR_IMGUI_ENABLED
        PushLayer(new ImGUIEditorMainLayer());
        PushLayer(new ImGUIFPSWindowLayer());
        PushLayer(new ImGUIDemoWindowLayer());
        PushLayer(new ImGUILogWindowLayer());
        #endif
        
        PushLayer(new DebugRenderingLayer());
    }

    ~Game() {}
};

namespace Blazar {

Application* CreateApplication() { return new Game(); }

}