#pragma once
#include <vector>

#include "EditorWindow.h"

namespace Blazar {
namespace Editor {

/// The Game's Integrated Editor.
class Editor {
   public:
    Editor();  ///< Constructor
    ~Editor();
    void render();  ///< Called when ImGUIRender is rendering
    void setup();   ///< Sets up the editor for usage.

    std::vector<EditorWindow*> m_windows;  ///< List of all open Windows
    std::vector<EditorWindow*> m_toClose;  ///< List of all windows to close at the end of the frame
    std::vector<EditorWindow*> m_toAdd;    ///< List of all windows to add at the beginning of the frame

    void                                   close_window(EditorWindow* window);
    template<class T, typename... Args> T* window_add_end(Args... args) {
        T* window = new T(args...);
        m_toAdd.push_back(window);
        window->m_state = EditorWindow::EditorWindowState::EDITOR_DOCKED;
        window->m_editorOrder  = editor_get_highest_order() + 1;
        window->m_editorScrollToWindow = true;
        return window;
    }

    void window_move_end(EditorWindow* window);

   private:
    template<typename T> void menu_spawn_unique() {
        for (auto* x : m_windows) {
            if (dynamic_cast<T*>(x)) { return; }
        }
        m_windows.push_back(new T());
    }

    int editor_get_highest_order() {
        int order = 0;
        for (auto* x : m_windows) {
            if (x->m_editorOrder > order) { order = x->m_editorOrder; }
        }

        return order;
    }
    int m_windowPadding      = 16;
    int m_innerWindowPadding = 4;
};

}  // namespace Editor
}  // namespace Blazar
