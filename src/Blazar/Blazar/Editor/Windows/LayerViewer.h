#pragma once

#include <imgui.h>

#include "Blazar/Editor/EditorWindow.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Layer/Layer.h"
#include "Blazar/Renderer/RenderCommand.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

namespace Blazar {
namespace Editor {

class LayerEditorWindow : public EditorWindow {
   public:
    bool show     = false;
    int  selected = -1;
    LayerEditorWindow() : EditorWindow("Layers") {}
    void RenderTable(Blazar::LayerUpdatePath path);
    void OnImGUIRender() override;
};

}  // namespace Editor
}  // namespace Blazar
#endif
