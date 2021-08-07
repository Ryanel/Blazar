#pragma once

#include <string>

#include "IComponent.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Blazar {
namespace Components {

struct NameComponent : IComponent{
    std::string name;  ///< The name of this entity

    NameComponent(std::string name) : name(name) {}
    void inspect() {
        ImGui::InputText("Name", &name);
    }

    COMPONENT_DEFINE_BODY();
};

}  // namespace Components
}  // namespace Blazar
