#pragma once

#include <spdlog/spdlog.h>
#include "Blazar/Log.h"
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {

class LogWindow : public EditorWindow {
   public:
    LogWindow(Editor* editor) : EditorWindow(editor, "Log", "Log", State::FREE_FLOATING) { m_useCustomWindow = true; }
    void render() override;

   private:
    void                      DisplayEntry(log_entry& e);
    bool                      m_Show           = true;
    bool                      m_ScrollToBottom = true;
    bool                      m_Options        = false;
    spdlog::level::level_enum m_filterSeverity = spdlog::level::trace;
    int                       m_EntriesToShow  = 200;
};

}  // namespace Editor
};  // namespace Blazar
