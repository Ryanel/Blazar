#include "Sandbox.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

#include "Blazar/Assets/Resource.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Blazar.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/RenderItem.h"
#include "Blazar/Renderer/Renderer.h"
#include "Tracy.hpp"

namespace Blazar {

void Sandbox::OnAttach() {
    ZoneScoped;
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

    Ref<VertexBuffer> sqr_vbo = VertexBuffer::Create(sqr_verts, sizeof(sqr_verts), sqr_layout);
    Ref<IndexBuffer> sqr_ibo = IndexBuffer::Create(sqr_indicies, sizeof(sqr_indicies));

    m_squareVAO = VertexArray::Create(sqr_vbo, sqr_ibo);
    m_squareVAO->Bind();

    m_shader = Shader::FromFile("Contents/Shaders/Simple");
    m_shader->SetName("Simple");
    m_shader->Bind();
    std::dynamic_pointer_cast<Blazar::OpenGLShader>(m_shader)->SetInt("u_Texture", 0);

    auto tex = ResourceManager::Get()->Load<Texture2D>("Textures/SampleTrans.png", true);
    m_texture = std::move(tex.value());

    // Camera
    auto& gameWindow = Application::Get().GetWindow();
    m_cameraController.reset(new OrthographicCamera(2.0f, gameWindow.GetAspect()));
    m_cameraController->SetPosition({0, 0, 0});
    m_cameraController->SetViewport(gameWindow.GetViewport());
}

void Sandbox::OnDetached() {}

void Sandbox::OnUpdate(Blazar::Timestep ts) {
    ZoneScoped;
    glm::mat4 sqr_pos = glm::translate(glm::mat4(1.0f), {0, 0, 0});

    m_cameraController->SetViewport(Application::Get().m_RenderViewport);
    m_cameraController->SetPosition({0, 0.00, 0});

    RenderCmd::BeginPass();
    RenderCmd::PassSetCamera(&(*m_cameraController));
    {
        RenderCmd::SetShader(m_shader);
        RenderCmd::UploadCameraProps();
        // RenderCmd::SetTranslation(sqr_pos);
        //std::dynamic_pointer_cast<Blazar::OpenGLShader>(m_shader)->SetMat4("u_Transform", sqr_pos);
        RenderCmd::SetShaderMat4("u_Transform", sqr_pos);
        RenderCmd::BindTexture(&(m_texture.get()));
        RenderCmd::DrawIndexed(m_squareVAO);
    }
    RenderCmd::EndPass();
}

void Sandbox::OnImGUIRender() { ZoneScoped; }

void Sandbox::OnEvent(Blazar::Events::Event& e) { ZoneScoped; }

}  // namespace Blazar
