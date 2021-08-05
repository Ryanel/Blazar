#pragma once

#include "Blazar/Config.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {

class InputEditorWindow : public EditorWindow {
   public:
    InputEditorWindow(Editor* editor) : EditorWindow(editor, "Input", "Input", State::FREE_FLOATING) {}
    void render() override;
};

}  // namespace Editor
}  // namespace Blazar
#endif
