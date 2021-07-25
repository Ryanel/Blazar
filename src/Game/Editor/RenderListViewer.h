#pragma once

#include <imgui.h>

#include "Blazar/Layer/Layer.h"
#include "Blazar/Renderer/RenderCommand.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

using namespace Blazar;

class RenderListWindowLayer : public Blazar::Layer {
   public:
    bool show = false;
    int selected = -1;
    RenderListWindowLayer() : Layer("Editor: Render Lists") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void ListItemRenderItem(RenderCommand& item, int index);
    void OnImGUIRender() override;
};

#endif
