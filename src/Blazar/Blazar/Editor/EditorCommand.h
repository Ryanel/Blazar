#pragma once

#include <string>
#include "Editor.h"
#include "EditorWindow.h"
#include "spdlog/fmt/fmt.h"

namespace Blazar {
namespace Editor {
class Editor;

class EditorCommand {
   public:
    EditorCommand(Editor* e) : m_editor(e) {}
    virtual void        undo()    = 0;
    virtual void        execute() = 0;
    virtual std::string name()    = 0;

   protected:
    Editor* m_editor;
};

template<typename T> class OpenWindowCommand : public EditorCommand {
   public:
    OpenWindowCommand(Editor* e) : EditorCommand(e) {}
    void undo() { m_editor->close_window(window); }
    void execute() { m_editor->window_add_end<T>(); }

    std::string name() { return fmt::format("Open Window {}", window->m_name); }
    Ref<T>      window;
};

template<typename T, typename U> class OpenWindowWithArgument : public EditorCommand {
   public:
    OpenWindowWithArgument(Editor* e, U arg1) : EditorCommand(e), m_arg1(arg1) {}
    void undo() {
        m_editor->close_window(window.get());
        window = nullptr;
    }
    void execute() { window = m_editor->window_add_end<T>(m_arg1); }

    std::string name() {
        if (window != nullptr) { return fmt::format("Open Window {}", window->m_title); }
        return "Open Window";
    }
    Ref<T> window = nullptr;

    U m_arg1;
};

}  // namespace Editor
}  // namespace Blazar
