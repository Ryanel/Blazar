#pragma once
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {

/// The editor FPS widget
class FPSWidgetWindowLayer : public EditorWindow {
   public:
    FPSWidgetWindowLayer(Editor* editor);
    void render() override;

    int numEntries = 20;
    int currentEntry = 0;
    std::vector<float> deltaTimes;
};

}  // namespace Editor
}  // namespace Blazar
