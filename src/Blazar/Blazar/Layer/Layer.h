#pragma once

#include "Blazar/Core.h"
#include "Blazar/Events/Events.h"
#include "Blazar/Time//Timestep.h"

namespace Blazar {

enum class LayerUpdatePath {
    Update,
    Render
};

class BLAZAR_API Layer {
   public:
    Layer(const std::string& name = "Unnamed Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetached() {}
    virtual void OnUpdate(Blazar::Timestep ts) {}
    virtual void OnEvent(Events::Event& ev) {}
    virtual void OnImGUIRender() {}
    inline const std::string& GetName() const { return m_DebugName; }

    LayerUpdatePath m_UpdatePath = LayerUpdatePath::Update;
   protected:
    std::string m_DebugName;
};
}  // namespace Blazar
