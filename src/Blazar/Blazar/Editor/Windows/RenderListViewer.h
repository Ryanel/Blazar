#pragma once

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/Renderer/RenderCommand.h"

namespace Blazar {
namespace Editor {
class RenderListWindowLayer : public EditorWindow {
   public:
    int  selected = -1;
    RenderListWindowLayer() : EditorWindow("Render Lists", "Render Lists") {m_useCustomWindow = true;}
    void ListItemRenderItem(RenderCommand& item, int index);
    void render(Editor* editor) override;
};

}  // namespace Editor
}  // namespace Blazar

#endif
