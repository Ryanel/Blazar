#pragma once

#include "Blazar/Core.h"
#include "Blazar/Layer/Layer.h"

namespace Blazar {

class ImGUILogWindowLayer : public Blazar::Layer {
   public:
    ImGUILogWindowLayer() : Layer("Editor: Log") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override;

   private:
    void DisplayEntry(log_entry& e);
    bool m_Show = true;
    bool m_ScrollToBottom = true;
    bool m_Options = false;
    spdlog::level::level_enum m_filterSeverity = spdlog::level::trace;
    int m_EntriesToShow = 200;
};
};  // namespace Blazar
