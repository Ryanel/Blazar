#pragma once

#include <string>
#include "Blazar/Core.h"
#include "Blazar/Time/Timestep.h"

namespace Blazar {

/// Layer Update Path
enum class LayerUpdatePath : int {
    None = 0,
    Update = 1,
    Render = 2,
    ImGui = 4,
    UpdateRender = 3,
    UpdateRenderImGui = 7
};

/// A part of the application that is run each frame.
class Layer {
   public:
    Layer(const std::string& name = "Unnamed Layer");  ///< Constructor
    virtual ~Layer();                                  ///< Deconstructor

    /// Called when this layer is attached
    virtual void OnAttach() {}
    /// Called when this layer is detached
    virtual void OnDetached() {}
    /// Called when a layer is updated
    virtual void OnUpdate(Blazar::Timestep ts) {}
    /// Called when a layer is rendered
    virtual void OnRender(Blazar::Timestep ts) {}
    /// Called when ImGUI renders this layer
    virtual void OnImGUIRender() {}
    /// Called when an event is propogated through the layers
    //virtual void OnEvent(Events::Event& ev) {}
    /// Returns the name of this layer
    inline const std::string& GetName() const { return m_DebugName; }
    /// Which paths this layer goes through
    LayerUpdatePath m_UpdatePath = LayerUpdatePath::None;

   protected:
    /// The name of this layer
    std::string m_DebugName;
};
}  // namespace Blazar
