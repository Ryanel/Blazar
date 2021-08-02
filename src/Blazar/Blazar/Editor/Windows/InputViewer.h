#pragma once

#include "Blazar/Config.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {

class InputEditorWindow : public EditorWindow {
   public:
    InputEditorWindow() : EditorWindow("Input") {}
    void render() override;

   private:
    bool show = false;
};

}  // namespace Editor
}  // namespace Blazar
#endif
