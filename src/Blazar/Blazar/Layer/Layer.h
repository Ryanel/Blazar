#pragma once

#include "Blazar/Core.h"
#include "Blazar/Events/Events.h"

namespace Blazar {
class BLAZAR_API Layer {
   public:
    Layer(const std::string& name = "Unnamed Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetached() {}
    virtual void OnUpdate() {}
    virtual void OnEvent(Event& ev) {}
    virtual void OnImGUIRender() {}
    inline const std::string& GetName() const { return m_DebugName; }

   protected:
    std::string m_DebugName;
};
}  // namespace Blazar
