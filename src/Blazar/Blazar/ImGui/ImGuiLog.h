#pragma once

#include "Blazar/Core.h"
#include "Blazar/Events/AppEvents.h"
#include "Blazar/Events/Events.h"
#include "Blazar/Events/KeyEvents.h"
#include "Blazar/Events/MouseEvents.h"
#include "Blazar/Layer/Layer.h"

namespace Blazar {

class ImGUILogWindowLayer : public Blazar::Layer {
   public:
    ImGUILogWindowLayer() : Layer("Log (ImGui)") {}
    void OnImGUIRender() override;

   private:
    void DisplayEntry(log_entry& e);
    bool m_Show = true;
    bool m_ScrollToBottom = true;
    bool m_Options = false;
    spdlog::level::level_enum m_filterSeverity = spdlog::level::trace;
    int m_EntriesToShow = 200;
};

}  // namespace Blazar
