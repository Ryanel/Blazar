#pragma once
#include <Blazar/Blazar.h>
#include <Blazar/Input/Keymap.h>
#include <Blazar/Renderer/RenderItem.h>
#include "Blazar/ImGui/CustomImGui.h"
#include <imgui.h>

#include "Tracy.hpp"
using namespace Blazar;

/// The editor FPS widget
class FPSWidgetWindowLayer : public Blazar::Layer {
   public:
    bool show = true; ///< Show this
    FPSWidgetWindowLayer() : Layer("Editor: FPS Widget") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override;
};
