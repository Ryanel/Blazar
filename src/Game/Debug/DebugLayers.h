#pragma once
#include <Blazar/Blazar.h>
#include <Blazar/Events/AppEvents.h>
#include <Blazar/Input/Keymap.h>

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
    ImGUIFPSWindowLayer() : Layer("ImGUI: FPS") {}
    void OnImGUIRender() override;
};

class ImGUIDemoWindowLayer : public Blazar::Layer {
   public:
    ImGUIDemoWindowLayer() : Layer("ImGUI Demo Window") {}
    void OnImGUIRender() override { ImGui::ShowDemoWindow(); }
};