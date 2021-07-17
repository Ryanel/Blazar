#pragma once

#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>
#include <Blazar/Input/Keymap.h>
#include <Blazar/Renderer/RenderItem.h>
#include "Blazar/ImGui/CustomImGui.h"
#include <imgui.h>

#include "Tracy.hpp"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

using namespace Blazar;

class RenderListWindowLayer : public Blazar::Layer {
   public:
    bool show = true;
    int selected = -1;
    RenderListWindowLayer() : Layer("Editor Render Lists") { m_UpdatePath = LayerUpdatePath::Render; }
    void ListItemRenderItem(RenderCommand& item, int index);
    void OnImGUIRender() override;
};

#endif