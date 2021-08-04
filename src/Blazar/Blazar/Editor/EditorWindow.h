#pragma once

#include <string>

namespace Blazar {
namespace Editor {
class Editor;

/// A Window that runs in the editor
class EditorWindow {
   public:
    enum class EditorWindowState : int {
        FREE_FLOATING    = 0,
        EDITOR_DOCKED    = 1,
        DOCKED_EXPANDED  = 2,
        DOCKED_MINIMIZED = 3,
    };
    EditorWindowState m_state = EditorWindowState::FREE_FLOATING;  ///< Current state of the window

    std::string m_name;                          ///< The name of this Window
    std::string m_title;                         ///< Window title of this window
    int         m_width                = 256;    ///< Desired width when editor-docked.
    int         m_height               = 300;    ///< Height of this window, set by the editor in docked mode;
    int         m_editorOrder          = -1;     ///< Order in the editor docked mode.
    bool        m_windowOpen           = true;   ///< Is the window currently open?
    bool        m_useCustomWindow      = false;  ///< Prevent draw_editor_Window from drawing a window for you.
    bool        m_expandRemainingSpace = false;
    int         m_width_min            = 256;
    int         m_width_expanded       = 400;

    bool m_allowsExpansion      = true;
    bool m_editorScrollToWindow = false;
    /// Constructor
    EditorWindow(std::string name, std::string title, EditorWindowState state = EditorWindowState::FREE_FLOATING)
        : m_name(name), m_title(title), m_state(state) {}

    /// Destructor
    virtual ~EditorWindow() {}

    /// Renders the Window
    virtual void render(Editor* editor) = 0;
    void         draw_editor_window(Editor* editor);
    void         show_close_control();
    void         unminimize(Editor* editor);
};

}  // namespace Editor
}  // namespace Blazar
