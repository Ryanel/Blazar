#pragma once

#include "Blazar/Blazar.h"

namespace Blazar {
class Editor : public Blazar::Layer {
   public:
    Editor();
    virtual void OnAttach() override;
    virtual void OnDetached() override;
    void Setup();
    virtual void OnImGUIRender() override;
    void OnEvent(Blazar::Events::Event& e) override;

    bool m_showImGuiStyleEditor = false;
};
}  // namespace Blazar
