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
    void undo() { m_editor->window_close(window); }
    void execute() { m_editor->window_add<T>(); }

    std::string name() { return fmt::format("Open Window {}", window->m_name); }
    Ref<T>      window;
};

template<typename T, typename U> class OpenWindowWithArgument : public EditorCommand {
   public:
    OpenWindowWithArgument(Editor* e, U arg1) : EditorCommand(e), m_arg1(arg1) {}
    void undo() {
        m_editor->window_close(window);
    }
    void execute() { window = m_editor->window_add<T>(m_arg1); }

    std::string name() {
        if (window != nullptr) { return fmt::format("Open Window {}", window->m_title); }
        return "Open Window";
    }
    Ref<T> window = nullptr;

    U m_arg1;
};

template<typename T> class CloseWindowCommand : public EditorCommand {
   public:
    CloseWindowCommand(Editor* e, Ref<T> win) : EditorCommand(e), window(win) {}
    void undo() { window.reset(m_editor->window_add<T>()); }
    void execute() {
        m_editor->window_close(window);
    }

    std::string name() {
        if (window != nullptr) { return fmt::format("Close Window {}", window->m_title); }
        return "Close Window";
    }

    Ref<T> window;
};

template<typename T, typename U> class CloseWindowWithArgument : public EditorCommand {
   public:
    CloseWindowWithArgument(Editor* e, Ref<T> win, U originalArgument)
        : EditorCommand(e), window(win), m_arg1(originalArgument) {}
    void undo() { window = m_editor->window_add<T>(m_arg1); }
    void execute() {
        m_editor->window_close(window);
    }

    std::string name() {
        if (window != nullptr) { return fmt::format("Close Window {}", window->m_title); }
        return "Close Window";
    }

    Ref<T> window;
    U      m_arg1;
};

}  // namespace Editor
}  // namespace Blazar
