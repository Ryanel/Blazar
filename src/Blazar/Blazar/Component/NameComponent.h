#pragma once

#include <string>

#include "IComponent.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Blazar {
namespace Components {

struct NameComponent {
    std::string name;
    NameComponent(std::string name) : name(name) {}
    void inspect() { ImGui::InputText("Name", &name); }

    REFLECTION_REFLECT(NameComponent);
    REFLECTION_DESCRIPTION("Holds the name of an entity");
    REFLECTION_METHOD("inspect");
    REFLECTION_MEMBER("name");
    REFLECTION_END();
};

}  // namespace Components
}  // namespace Blazar
