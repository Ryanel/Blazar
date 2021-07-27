#pragma once

#include <string>

namespace Blazar {
namespace Editor {

class EditorWindow {
   public:
    std::string m_name;
    bool        m_active;

    EditorWindow(std::string name) : m_active(true), m_name(name) {}
    virtual ~EditorWindow() {}
    virtual void OnImGUIRender() = 0;
};

}  // namespace Editor
}  // namespace Blazar
