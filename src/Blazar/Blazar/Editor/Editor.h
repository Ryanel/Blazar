#pragma once
#include <vector>

#include "EditorWindow.h"

namespace Blazar {
namespace Editor {

/// The Game's Integrated Editor.
class Editor {
   public:
    Editor();              ///< Constructor
    void RenderWindow();  ///< Called when ImGUIRender is rendering
    void Setup();          ///< Sets up the editor for usage.

    std::vector<EditorWindow*> m_windows;  ///< List of all open Windows
};

}  // namespace Editor
}  // namespace Blazar
