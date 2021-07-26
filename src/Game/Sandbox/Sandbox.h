#pragma once

#include "Blazar/Assets/Resource.h"
#include "Blazar/Layer/Layer.h"
#include "Blazar/Memory.h"
#include "Blazar/Renderer/Primitives/Quad.h"

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
    void         OnUpdate(Blazar::Timestep ts) override;
    void         OnRender(Blazar::Timestep ts) override;
    virtual void OnImGUIRender() override;

   private:
    Blazar::Ref<Shader>              m_shader;
    Ref<Blazar::Resource<Texture2D>> m_texture;
    Blazar::Ref<Quad>                m_quad = nullptr;
    Blazar::Ref<OrthographicCamera>  m_cameraController;
};
}  // namespace Blazar
