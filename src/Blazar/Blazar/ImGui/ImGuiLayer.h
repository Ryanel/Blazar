#pragma once

#ifdef BLAZAR_IMGUI_ENABLED

#include "Blazar/Core.h"
#include "Blazar/Layer/Layer.h"


namespace Blazar {
class BLAZAR_API ImGuiLayer : public Layer {
   public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    void OnAttach() override;
    void OnDetached() override;

    void Begin();
    void End(bool draw = true);

    inline const std::string& GetName() const { return m_DebugName; }

   private:
    float m_Time = 0;
};
}  // namespace Blazar

#endif
