#pragma once

#include "Blazar/Blazar.h"

#include "Blazar/Renderer/OrthographicCamera.h"
#include "Blazar/Renderer/Shader.h"
#include "Blazar/Renderer/Texture.h"
#include "Blazar/Renderer/VertexArray.h"

namespace Blazar {
class Sandbox : public Blazar::Layer {
   public:
    Sandbox() : Blazar::Layer("Sandbox") { m_UpdatePath = LayerUpdatePath::Render; }
    virtual void OnAttach() override;
    virtual void OnDetached() override;

    void OnUpdate(Blazar::Timestep ts) override;
    virtual void OnImGUIRender() override;
    void OnEvent(Blazar::Events::Event& e) override;

   private:
    Blazar::Ref<VertexArray> m_squareVAO;
    Blazar::Ref<Shader> m_shader;
    Blazar::Ref<Texture> m_texture;
    Blazar::Ref<OrthographicCamera> m_cameraController;
};
}  // namespace Blazar
