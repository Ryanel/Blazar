#pragma once
#include "Blazar/Layer/Layer.h"

using namespace Blazar;

/// The editor FPS widget
class FPSWidgetWindowLayer : public Blazar::Layer {
   public:
    bool show = true;  ///< Show this
    FPSWidgetWindowLayer() : Layer("Editor: FPS Widget") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override;
};
