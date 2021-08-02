#pragma once

#include <string>

namespace Blazar {
namespace Editor {

/// A Window that runs in the editor
class EditorWindow {
   public:
    std::string m_name;    ///< The name of this Window
    bool        m_active;  ///< Is this Window open?

    /// Constructor
    EditorWindow(std::string name) : m_active(true), m_name(name) {}

    /// Destructor
    virtual ~EditorWindow() {}

    /// Renders the Window
    virtual void render() = 0;
};

}  // namespace Editor
}  // namespace Blazar
