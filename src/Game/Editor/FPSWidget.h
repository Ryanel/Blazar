#pragma once
#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>
#include <Blazar/Input/Keymap.h>
#include <Blazar/Renderer/RenderItem.h>
#include "Blazar/ImGui/CustomImGui.h"
#include <imgui.h>

#include "Tracy.hpp"
using namespace Blazar;

class FPSWidgetWindowLayer : public Blazar::Layer {
   public:
    bool show = true;
    int selected = -1;
    FPSWidgetWindowLayer() : Layer("Editor: FPS Widget") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override;
};
