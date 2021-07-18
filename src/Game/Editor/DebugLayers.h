#pragma once
#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>
#include <Blazar/Input/Keymap.h>
#include "Blazar/ImGui/CustomImGui.h"
#include <imgui.h>

#include "Tracy.hpp"

using namespace Blazar;

#ifdef BLAZAR_IMGUI_ENABLED

class ImGUIDemoWindowLayer : public Blazar::Layer {
   public:
    bool show = false;
    ImGUIDemoWindowLayer() : Layer("Editor: ImGui Development Window") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override {
        ZoneScoped;
        ImGUI_MainMenu_Toggle_Simple("[Development]", "ImGUI Demo", "", this->show, true);
        if (!this->show) { return; }
        ImGui::ShowDemoWindow(&this->show);
    }
};
#endif