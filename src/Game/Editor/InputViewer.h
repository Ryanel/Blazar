#pragma once

#include "Blazar/Config.h"
#include "Blazar/Layer/Layer.h"

#ifdef BLAZAR_CFG_DEV_RENDER_COMMAND_INTROSPECTION

using namespace Blazar;

class InputEditorWindow : public Blazar::Layer {
   public:
    InputEditorWindow() : Layer("Editor: Input Window") { m_UpdatePath = LayerUpdatePath::ImGui; }
    void OnImGUIRender() override;

   private:
    bool show = false;
};

#endif
