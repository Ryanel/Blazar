#pragma once
#include <vector>

#include "Blazar/Layer/Layer.h"
#include "EditorWindow.h"

namespace Blazar {
namespace Editor {

class Editor : public Blazar::Layer {
   public:
    Editor();
    virtual void OnAttach() override;
    virtual void OnDetached() override;
    virtual void OnImGUIRender() override;
    void         Setup();

    std::vector<EditorWindow*> m_windows;

    bool m_showImGuiStyleEditor = false;
};

}  // namespace Editor
}  // namespace Blazar
