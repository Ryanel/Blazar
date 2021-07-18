#pragma once

#include "Blazar/Blazar.h"

#include "Blazar/Renderer/Cameras/OrthographicCamera.h"
#include "Blazar/Renderer/Primitives/Shader.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Renderer/Primitives/VertexArray.h"

#include "Blazar/Assets/Resource.h"

namespace Blazar {
class Sandbox : public Blazar::Layer {
   public:
    Sandbox() : Blazar::Layer("Sandbox") { m_UpdatePath = LayerUpdatePath::UpdateRender; }
    virtual void OnAttach() override;
    virtual void OnDetached() override;

    void OnUpdate(Blazar::Timestep ts) override;
    void OnRender(Blazar::Timestep ts) override;
    virtual void OnImGUIRender() override;
    void OnEvent(Blazar::Events::Event& e) override;

   private:
    Blazar::Ref<VertexArray> m_squareVAO;
    Blazar::Ref<Shader> m_shader;
    Blazar::Resource<Texture2D> m_texture;
    Blazar::Ref<OrthographicCamera> m_cameraController;
};
}  // namespace Blazar
