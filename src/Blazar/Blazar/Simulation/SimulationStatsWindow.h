#pragma once

#include "Blazar/Config.h"
#include "Blazar/Layer/Layer.h"

namespace Blazar {
class SimulationStatsWindow : public Blazar::Layer {
   public:
    SimulationStatsWindow() : Layer("Editor: Simulation Window") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override;

   private:
    bool show = true;
};

}  // namespace Blazar
