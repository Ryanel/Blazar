#pragma once

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/RenderCommand.h"

namespace Blazar {
namespace Editor {
class RenderListViewer : public EditorWindow {
   public:
    int  selected = -1;
    RenderListViewer(Editor* editor) : EditorWindow(editor, "Render Lists", "Render Lists") {}
    void ListItemRenderItem(RenderCommand& item, int index);
    void render() override;
};

}  // namespace Editor
}  // namespace Blazar

#endif
