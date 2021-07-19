#pragma once

#include <Blazar/Blazar.h>
#include <Blazar/Input/Keymap.h>
#include "Blazar/ImGui/CustomImGui.h"
#include <imgui.h>

#include "Tracy.hpp"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

using namespace Blazar;

class InputEditorWindow : public Blazar::Layer {
   public:
    bool show = true;
    int selected = -1;
    InputEditorWindow() : Layer("Editor: Input Window") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override;
};

#endif