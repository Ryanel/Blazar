#pragma once

#include "Blazar/Layer/Layer.h"

namespace Blazar {
class Editor : public Blazar::Layer {
   public:
    Editor();
    virtual void OnAttach() override;
    virtual void OnDetached() override;
    void Setup();
    virtual void OnImGUIRender() override;

    bool m_showImGuiStyleEditor = false;
};
}  // namespace Blazar
