#pragma once

#include "Blazar/Config.h"
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {
class GameWindow : public EditorWindow {
   public:
    GameWindow() : EditorWindow("Game") {}
    void OnImGUIRender() override;

   private:
    bool show = true;
};
}  // namespace Editor
}  // namespace Blazar
