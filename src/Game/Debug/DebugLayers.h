#pragma once
#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>
#include <Blazar/Input/Keymap.h>
#include "Blazar/ImGui/CustomImGui.h"
#include <imgui.h>

using namespace Blazar;

class LogEventsLayer : public Blazar::Layer {
   public:
    LogEventsLayer() : Layer("Debug Event Layer") {}
    void OnUpdate() override {}
    void OnEvent(Blazar::Events::Event& event) override;
};

class ImGUIFPSWindowLayer : public Blazar::Layer {
   public:
    bool show = true;
    ImGUIFPSWindowLayer() : Layer("ImGUI: FPS") {}
    void OnImGUIRender() override;
};

class ImGUIDemoWindowLayer : public Blazar::Layer {
   public:
    bool show = false;
    ImGUIDemoWindowLayer() : Layer("ImGUI Demo Window") {}
    void OnImGUIRender() override {
        ImGUI_MainMenu_Toggle_Simple("Windows", "ImGUI Demo", "", this->show, true);

        if (!this->show) { return; }

        ImGui::ShowDemoWindow(&this->show);
    }
};