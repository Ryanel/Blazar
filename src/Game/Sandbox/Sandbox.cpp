#include "Sandbox.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

#include "Blazar/Application.h"
#include "Blazar/Assets/Resource.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Blazar.h"
#include "Blazar/Log.h"
#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Cameras/OrthographicCamera.h"
#include "Blazar/Renderer/Primitives/Buffer.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/Primitives/Shader.h"
#include "Blazar/Renderer/Primitives/VertexArray.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/RenderCommand.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/Simulation.h"
#include "Blazar/VFS/VFSFilesystem.h"
#include "Tracy.hpp"

#include "Blazar/Component/MeshComponent.h"
#include "Blazar/Component/RenderTransform.h"
#include "Components/Transform.h"

namespace Blazar {

void Sandbox::OnAttach() {
    ZoneScoped;
    // Square

    Application& app = Application::Get();
    auto&        sim = app.m_Simulation;
    sim->world.prepare<Transform>();
    sim->world.prepare<Blazar::MeshComponent>();
    sim->world.prepare<Blazar::RenderTransform>();
    m_quad = std::make_shared<Quad>();

    entt::entity entity = sim->world.create();

    sim->world.emplace<Transform>(entity, glm::vec3(-0.75f, 0.0f, 0.0f));
    sim->world.emplace<Blazar::MeshComponent>(entity, m_quad->vao);
    sim->world.emplace<Blazar::RenderTransform>(entity , glm::vec3(-0.75f, 0.0f, 0.0f));

    entt::entity entity2 = sim->world.create();

    sim->world.emplace<Transform>(entity2, glm::vec3(0.75f, 0.0f, 0.0f));
    sim->world.emplace<Blazar::MeshComponent>(entity2, m_quad->vao);
    sim->world.emplace<Blazar::RenderTransform>(entity2 , glm::vec3(0.75f, 0.0f, 0.0f));

    m_shader = Shader::FromFile("Contents/Data/Shaders/Simple");
    m_shader->SetName("Simple");
    m_shader->Bind();
    std::dynamic_pointer_cast<Blazar::OpenGLShader>(m_shader)->SetInt("u_Texture", 0);

    Blazar::TextureProperties texprops;
    texprops.filtering = TextureFilterMode::None;
    m_texture          = Texture2D::Load("/Data/Textures/SampleTrans.png", texprops);

    // Camera
    auto& gameWindow = Application::Get().GetWindow();
    m_cameraController.reset(new OrthographicCamera(2.0f, gameWindow.GetAspect()));
    m_cameraController->SetPosition({0, 0, 0});
    m_cameraController->SetViewport(gameWindow.GetViewport());
}

void Sandbox::OnDetached() {}
void Sandbox::OnUpdate(Blazar::Timestep ts) {}
void Sandbox::OnRender(Blazar::Timestep ts) {
    Application& app  = Application::Get();
    auto&        sim  = app.m_Simulation;
    auto         view = sim->world.view<Blazar::RenderTransform, Blazar::MeshComponent>();
    m_cameraController->SetViewport(app.m_RenderViewport);



    for (auto e : view) {
        auto [transform, mesh] = view.get(e);
        glm::mat4 sqr_pos = glm::translate(glm::mat4(1.0f), transform.position);

        RENDER_SUBMIT(RenderCmd::BeginPass());
        RENDER_SUBMIT(RenderCmd::PassSetCamera(m_cameraController));
        RENDER_SUBMIT(RenderCmd::SetShader(m_shader));
        RENDER_SUBMIT(RenderCmd::UploadCameraProps());
        RENDER_SUBMIT(RenderCmd::SetShaderMat4("u_Transform", sqr_pos));
        RENDER_SUBMIT(RenderCmd::BindTexture(m_texture->data()));
        RENDER_SUBMIT(RenderCmd::DrawIndexed(mesh.mesh));
        RENDER_SUBMIT(RenderCmd::EndPass());
    }
}
void Sandbox::OnImGUIRender() { ZoneScoped; }

}  // namespace Blazar
