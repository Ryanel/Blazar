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
    Editor();  ///< Constructor
    ~Editor();
    void render();  ///< Called when ImGUIRender is rendering
    void setup();   ///< Sets up the editor for usage.

    void close_window(EditorWindow* window);

    template<class T, typename... Args> Ref<T> window_add_end(Args... args) {
        Ref<T> window = std::make_shared<T>(this, args...);
        m_toAdd.push_back(window);
        window->m_editorOrder          = editor_get_highest_order() + 1;
        window->m_editorScrollToWindow = true;
        return window;
    }

    void window_move_end(EditorWindow* window);

    void undo();
    void redo();
    void perform(EditorCommand* c);

   private:
    std::vector<Ref<EditorWindow>> m_windows;  ///< List of all open Windows
    std::vector<Ref<EditorWindow>> m_toClose;  ///< List of all windows to close at the end of the frame
    std::vector<Ref<EditorWindow>> m_toAdd;    ///< List of all windows to add at the beginning of the frame
    std::deque<EditorCommand*>     m_editorCommandList;
    size_t                         m_editorCommandPtr = 0;

    Ref<EditorWindow> find_ref_from_list(EditorWindow* needle, std::vector<Ref<EditorWindow>> haystack) {
        for (auto x : haystack) {
            if (x.get() == needle) { return x; }
        }

        return nullptr;
    }

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
    int m_windowPadding      = 16;
    int m_innerWindowPadding = 4;
};

}  // namespace Editor
}  // namespace Blazar
