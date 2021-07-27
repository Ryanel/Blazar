#pragma once

#include "Blazar/Config.h"
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {
class SimulationStatsWindow : public EditorWindow {
   public:
    SimulationStatsWindow() : EditorWindow("Simulation Stats") {}
    void OnImGUIRender() override;

   private:
    bool show = true;
};
}  // namespace Editor
}  // namespace Blazar
