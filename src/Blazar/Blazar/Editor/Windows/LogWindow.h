#pragma once

#include <spdlog/spdlog.h>
#include "Blazar/Log.h"
#include "Blazar/Editor/EditorWindow.h"

namespace Blazar {
namespace Editor {

class LogWindow : public EditorWindow {
   public:
    LogWindow() : EditorWindow("Log") { ; }
    void RenderWindow() override;

   private:
    void                      DisplayEntry(log_entry& e);
    bool                      m_Show           = true;
    bool                      m_ScrollToBottom = true;
    bool                      m_Options        = false;
    spdlog::level::level_enum m_filterSeverity = spdlog::level::trace;
    int                       m_EntriesToShow  = 200;

    bool focused      = false;
    bool childFocused = false;
};

}  // namespace Editor
};  // namespace Blazar
