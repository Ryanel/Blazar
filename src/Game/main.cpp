#define BLAZAR_CREATE_APPLICATION
#include "Blazar/Application.h"

#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Tracy.hpp"

#include "Blazar/Assets/Resource.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Blazar.h"
#include "Blazar/Editor/Editor.h"
#include "Blazar/Log.h"
#include "Blazar/Renderer/Cameras/OrthographicCamera.h"
#include "Blazar/Renderer/Primitives/Buffer.h"
#include "Blazar/Renderer/Primitives/Quad.h"
#include "Blazar/Renderer/Primitives/RenderTexture.h"
#include "Blazar/Renderer/Primitives/Shader.h"
#include "Blazar/Renderer/Primitives/VertexArray.h"
#include "Blazar/Renderer/RenderCmd.h"
#include "Blazar/Renderer/RenderCommand.h"
#include "Blazar/Renderer/Renderer.h"
#include "Blazar/Simulation/Scene.h"
#include "Blazar/Simulation/SceneManager.h"
#include "Blazar/Utility/Path.h"
#include "Blazar/VFS/VFSFilesystem.h"
#include "Tracy.hpp"

#include "Blazar/Component/MeshComponent.h"
#include "Blazar/Component/RenderTransform.h"
#include "Blazar/Component/TextureComponent.h"
#include "Components/Transform.h"

#include "Blazar/Entry.h"

using namespace Blazar;
using namespace Blazar::Components;

class TestScene : public Blazar::Scenes::Scene {
   public:
    TestScene() : Scene("Test Scene") {
        auto& world = registry();

        m_quad = std::make_shared<Quad>();

        Blazar::TextureProperties texprops;
        texprops.filtering = TextureFilterMode::None;
        m_texture          = Texture2D::Load("/Data/Textures/SampleTrans.png", texprops);
        m_texture2         = Texture2D::Load("/Data/Textures/dante.png", texprops);

        /// Prepare the world for components we know it'll need.
        world.prepare<Transform>();
        world.prepare<MeshComponent>();
        world.prepare<RenderTransform>();
        world.prepare<TextureComponent>();

        entt::entity entity  = world.create();
        entt::entity entity2 = world.create();

        // Entity 1
        world.emplace<Transform>(entity, glm::vec3(-0.75f, 0.0f, 0.0f));
        world.emplace<MeshComponent>(entity, m_quad->vao);
        world.emplace<TextureComponent>(entity, m_texture->data());
        world.emplace<RenderTransform>(entity, glm::vec3(-0.75f, 0.0f, 0.0f));

        // Entity 2
        world.emplace<Transform>(entity2, glm::vec3(0.75f, 0.0f, 0.0f));
        world.emplace<MeshComponent>(entity2, m_quad->vao);
        world.emplace<TextureComponent>(entity2, m_texture2->data());
        world.emplace<RenderTransform>(entity2, glm::vec3(0.75f, 0.0f, 0.0f));

        m_shader = Shader::Load("/Data/Shaders/Simple");
        m_shader->SetName("Simple");
        m_shader->Bind();
        m_shader->SetInt("u_Texture", 0);

        // Camera
        auto& gameWindow = Application::Get().GetWindow();
        m_cameraController.reset(new OrthographicCamera(2.0f, gameWindow.GetAspect()));
        m_cameraController->SetPosition({0, 0, 0});
        m_cameraController->SetViewport(gameWindow.GetViewport());
    }
    virtual ~TestScene() {}

    virtual void OnUpdate(Timestep& ts) {}

    virtual void OnRender(Timestep& ts) {
        Application& app  = Application::Get();
        auto         view = registry().view<RenderTransform, MeshComponent, TextureComponent>();
        m_cameraController->SetViewport(app.m_RenderViewport);

        std::vector<RenderCommand> cmds;
        cmds.emplace_back(RenderCmd::BeginPass());
        cmds.emplace_back(RenderCmd::PassSetCamera(m_cameraController));
        cmds.emplace_back(RenderCmd::SetShader(m_shader));

        for (auto ent : view) {
            auto [transform, mesh, tex] = view.get(ent);
            glm::mat4 transform_mat     = glm::translate(glm::mat4(1.0f), transform.position);
            cmds.emplace_back(RenderCmd::UploadCameraProps());
            cmds.emplace_back(RenderCmd::SetShaderMat4("u_Transform", transform_mat));
            cmds.emplace_back(RenderCmd::BindTexture(tex.tex));
            cmds.emplace_back(RenderCmd::DrawIndexed(mesh.mesh));
        }

        cmds.emplace_back(RenderCmd::EndPass());
        Renderer::SubmitList(cmds);
    }

    Ref<Resource<Texture2D>> m_texture;
    Ref<Resource<Texture2D>> m_texture2;
    Ref<Shader>              m_shader;
    Ref<Quad>                m_quad = nullptr;
    Ref<OrthographicCamera>  m_cameraController;
};

class Game : public Blazar::Application {
   public:
    Game() {
        ZoneScoped;
        m_name = "Blazar";

        m_editor = new Editor::Editor();
        m_editor->Setup();

        Setup();
    }

    ~Game() {}

    // Game Lifecycle methods
    void Setup() { this->m_SceneManager->SetMainScene(new TestScene()); }
    void OnUpdate() override {}
    void OnRender() override {}
};

namespace Blazar {

Application* CreateApplication() {
    tracy::SetThreadName("Main Thread");

    auto* rm = ResourceManager::Get();
    rm->m_vfs->add_mountpoint(new VFS::FileSystem("/Data/", "Contents/Data/", true));
    rm->m_vfs->add_mountpoint(new VFS::FileSystem("/Editor/", "Contents/Editor/", true));
    rm->m_vfs->refresh();

    return new Game();
}

}  // namespace Blazar
