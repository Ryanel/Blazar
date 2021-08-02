#pragma once
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {

/// The editor FPS widget
class FPSWidgetWindowLayer : public EditorWindow {
   public:
    bool show = true;  ///< Show this
    FPSWidgetWindowLayer() : EditorWindow("FPS Widget") {}
    void render() override;
};

}  // namespace Editor
}  // namespace Blazar
