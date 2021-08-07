#pragma once

#include <glm/glm.hpp>
#include "imgui.h"

namespace Blazar {
namespace Components {

struct IComponent {
    virtual void   inspect() { ImGui::Text("No Inspector"); }
    virtual size_t size_of() { return sizeof(*this); }
};

#define COMPONENT_DEFINE_BODY() virtual size_t size_of() { return sizeof(*this); }
}  // namespace Components
}  // namespace Blazar
