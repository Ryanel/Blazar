#pragma once

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/RenderCommand.h"

namespace Blazar {
namespace Editor {
class RenderListWindowLayer : public EditorWindow {
   public:
    bool show     = true;
    int  selected = -1;
    RenderListWindowLayer() : EditorWindow("Render Lists") {}
    void ListItemRenderItem(RenderCommand& item, int index);
    void render() override;
};

}  // namespace Editor
}  // namespace Blazar

#endif
