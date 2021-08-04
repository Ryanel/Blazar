#pragma once

#include "Blazar/Config.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {

class InputEditorWindow : public EditorWindow {
   public:
    InputEditorWindow() : EditorWindow("Input", "Input", EditorWindowState::FREE_FLOATING) {}
    void render(Editor* editor) override;
};

}  // namespace Editor
}  // namespace Blazar
#endif
