#pragma once
#include <deque>
#include <vector>
#include "Blazar/Memory.h"

#include "EditorWindow.h"

namespace Blazar {
namespace Editor {

class EditorCommand;

/// The Game's Integrated Editor.
class Editor {
   public:
    // Lifecycle Methods
    // ------------------------------------------------------------
    Editor();       ///< Constructor
    ~Editor();      ///< Destructor
    void setup();   ///< Sets up the editor for usage.
    void render();  ///< Called when ImGUIRender is rendering
    void main_menu();

    // Window Operations
    // ------------------------------------------------------------
    template<class T, typename... Args> Ref<T> window_add(Args... args) {
        Ref<T> window = std::make_shared<T>(this, args...);
        m_windows_to_open.push_back(window);
        window->m_editorOrder          = editor_get_highest_order() + 1;
        window->m_editorScrollToWindow = true;
        return window;
    }

    template<class T, typename... Args> Ref<T> window_add(Ref<T> window) {
        m_windows_to_open.push_back(window);
        window->m_editorOrder          = editor_get_highest_order() + 1;
        window->m_editorScrollToWindow = true;
        return window;
    }

    void window_close(Ref<EditorWindow> window);
    void window_move_end(Ref<EditorWindow> window);

    // Command interface
    // ------------------------------------------------------------
    void undo();
    void redo();
    void perform(EditorCommand* c);

   private:
    template<typename T, typename... Args> void menu_spawn_unique(Args... args) {
        for (auto x : m_windows) {
            if (dynamic_cast<T*>(x.get())) { return; }
        }
        m_windows.push_back(std::make_shared<T>(this, args...));
    }

    int editor_get_highest_order() {
        int order = 0;
        for (auto x : m_windows) {
            if (x->m_editorOrder > order) { order = x->m_editorOrder; }
        }

        return order;
    }

   private:
    int m_windowPadding      = 16;
    int m_innerWindowPadding = 16;

    std::vector<Ref<EditorWindow>> m_windows;            ///< List of all windows.
    std::vector<Ref<EditorWindow>> m_windows_open;       ///< List of all open Windows
    std::vector<Ref<EditorWindow>> m_windows_toClose;    ///< List of all windows to close at the end of the frame
    std::vector<Ref<EditorWindow>> m_windows_to_open;    ///< List of all windows to add at the beginning of the frame
    std::deque<EditorCommand*>     m_editorCommandList;  ///< List of commands
    size_t                         m_editorCommandPtr = 0;  ///< Command Pointer
};

}  // namespace Editor
}  // namespace Blazar
