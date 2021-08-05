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
    virtual void undo() { m_editor->close_window(window); }
    void         execute() { m_editor->window_add_end<T>(); }
    std::string  name() { return fmt::format("Open Window {}", window->m_name); }
    Ref<T>       window;
};

template<typename T> class OpenWindowWithPathCommand : public EditorCommand {
   public:
    OpenWindowWithPathCommand(Editor* e, std::string path) : EditorCommand(e), m_path(path) {}
    virtual void undo() {
        m_editor->close_window(window.get());
        window = nullptr;
    }
    void        execute() { window = m_editor->window_add_end<T>(m_path); }
    std::string name() {
        if (window != nullptr) { return fmt::format("Open Window (with path) {} ({})", window->m_title, m_path); }
        return "Open Window";
    }
    Ref<T>      window = nullptr;
    std::string m_path;
};

}  // namespace Editor
}  // namespace Blazar
