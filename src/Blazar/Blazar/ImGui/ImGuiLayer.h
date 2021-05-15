#pragma once

#include "Blazar/Core.h"
#include "Blazar/Events/AppEvents.h"
#include "Blazar/Events/Events.h"
#include "Blazar/Events/KeyEvents.h"
#include "Blazar/Events/MouseEvents.h"
#include "Blazar/Layer/Layer.h"

namespace Blazar {
class BLAZAR_API ImGuiLayer : public Layer {
   public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    void OnAttach() override;
    void OnDetached() override;
    void OnUpdate() override;
    void OnEvent(Events::Event& ev) override;

    void Begin();
    void End();

    inline const std::string& GetName() const { return m_DebugName; }

   private:
    float m_Time = 0;
};
}  // namespace Blazar
