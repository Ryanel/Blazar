#pragma once

#include <glm/glm.hpp>
#include "IComponent.h"

namespace Blazar {
namespace Components {

struct Transform {
    glm::vec3 position;

    Transform() = default;
    Transform(glm::vec3 pos) : position(pos) {}
    Transform(Transform&& t) noexcept {
        this->position = t.position;
        t.position     = glm::vec3();
    }

    Transform& operator=(Transform&& other) noexcept {
        this->position = other.position;
        other.position = glm::vec3();
        return *this;
    }

    void inspect() { 
        ImGui::InputFloat3("Position", &position.x);
    }

    REFLECTION_REFLECT(Transform);
    REFLECTION_DESCRIPTION("Contains a 3D position in space");
    REFLECTION_MEMBER("position");
    REFLECTION_METHOD("inspect");
    REFLECTION_END();
};

}  // namespace Components
}  // namespace Blazar
