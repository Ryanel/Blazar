#pragma once

#include "Blazar/Core.h"
#include "Blazar/Events/Events.h"
#include "Blazar/Time//Timestep.h"

namespace Blazar {

enum class LayerUpdatePath : int { 
    None = 0, Update = 1, Render = 2, ImGui = 4 ,
    UpdateRender = 3,
    UpdateRenderImGui = 7
};

class BLAZAR_API Layer {
   public:
    Layer(const std::string& name = "Unnamed Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetached() {}
    virtual void OnUpdate(Blazar::Timestep ts) {}
    virtual void OnRender(Blazar::Timestep ts) {}
    virtual void OnImGUIRender() {}
    virtual void OnEvent(Events::Event& ev) {}
    inline const std::string& GetName() const { return m_DebugName; }

    LayerUpdatePath m_UpdatePath = LayerUpdatePath::None;

   protected:
    std::string m_DebugName;
};
}  // namespace Blazar
