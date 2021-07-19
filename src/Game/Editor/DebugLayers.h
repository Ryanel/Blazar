#pragma once
#include <imgui.h>
#include "Blazar/Blazar.h"
#include "Blazar/ImGui/CustomImGui.h"
#include "Blazar/Input/Keymap.h"
#include "Tracy.hpp"

using namespace Blazar;

class ImGUIDemoWindowLayer : public Blazar::Layer {
   public:
    bool show = false;
    ImGUIDemoWindowLayer() : Layer("Editor: ImGui Development Window") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override {
        ZoneScoped;
        ImGUI_MainMenu_Toggle_Simple("[Development]", "ImGUI Demo", "", this->show, true);
        if (this->show) { ImGui::ShowDemoWindow(&this->show); }
    }
};
