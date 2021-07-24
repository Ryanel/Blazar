#pragma once

#include <imgui.h>

#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Layer/Layer.h"
#include "Blazar/Renderer/RenderCommand.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

using namespace Blazar;

class LayerEditorWindow : public Blazar::Layer {
   public:
    bool show = true;
    int selected = -1;
    LayerEditorWindow() : Layer("Editor: Layer Window") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void RenderTable(LayerUpdatePath path);
    void OnImGUIRender() override;
};

#endif
