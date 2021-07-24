#pragma once

#include "Blazar/Memory.h"
#include "Blazar/Layer/Layer.h"
#include "Blazar/Assets/Resource.h"

namespace Blazar {

class Texture2D;
class VertexArray;
class OrthographicCamera;
class Shader;

class Sandbox : public Blazar::Layer {
   public:
    Sandbox() : Blazar::Layer("Sandbox") { m_UpdatePath = LayerUpdatePath::UpdateRender; }
    virtual void OnAttach() override;
    virtual void OnDetached() override;

    void OnUpdate(Blazar::Timestep ts) override;
    void OnRender(Blazar::Timestep ts) override;
    virtual void OnImGUIRender() override;

   private:
    Blazar::Ref<VertexArray> m_squareVAO;
    Blazar::Ref<Shader> m_shader;
    Blazar::Resource<Texture2D> m_texture;
    Blazar::Ref<OrthographicCamera> m_cameraController;
};
}  // namespace Blazar
