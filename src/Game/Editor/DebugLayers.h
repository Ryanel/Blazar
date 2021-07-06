#pragma once
#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>
#include <Blazar/Input/Keymap.h>
#include "Blazar/ImGui/CustomImGui.h"
#include <imgui.h>

using namespace Blazar;

#ifdef BLAZAR_IMGUI_ENABLED
class ImGUIFPSWindowLayer : public Blazar::Layer {
   public:
    bool show = true;
    ImGUIFPSWindowLayer() : Layer("ImGUI: FPS") { m_UpdatePath = LayerUpdatePath::Render; }
    void OnImGUIRender() override;
};

class ImGUIDemoWindowLayer : public Blazar::Layer {
   public:
    bool show = false;
    ImGUIDemoWindowLayer() : Layer("ImGUI Demo Window") { m_UpdatePath = LayerUpdatePath::Render; }
    void OnImGUIRender() override {
        BLAZAR_PROFILE_FUNCTION();
        ImGUI_MainMenu_Toggle_Simple("Windows", "ImGUI Demo", "", this->show, true);

        if (!this->show) { return; }

        ImGui::ShowDemoWindow(&this->show);
    }
};
#endif