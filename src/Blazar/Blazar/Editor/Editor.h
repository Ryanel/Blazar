#pragma once
#include <vector>

#include "EditorWindow.h"

namespace Blazar {
namespace Editor {

class Editor {
   public:
    Editor();
    void OnAttach();
    void OnDetached();
    void OnImGUIRender();
    void Setup();

    std::vector<EditorWindow*> m_windows;

    bool m_showImGuiStyleEditor = false;
};

}  // namespace Editor
}  // namespace Blazar
