#pragma once

#include "Blazar/Config.h"
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {
class GameWindow : public EditorWindow {
   public:
    GameWindow(Editor* editor) : EditorWindow(editor, "Game", "Game", State::FREE_FLOATING) { m_useCustomWindow = true; }
    void render() override;

   private:
    bool show = true;
};
}  // namespace Editor
}  // namespace Blazar
