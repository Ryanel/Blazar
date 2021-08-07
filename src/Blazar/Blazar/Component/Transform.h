#pragma once

#include <glm/glm.hpp>
#include "IComponent.h"

namespace Blazar {
namespace Components {

struct Transform : public IComponent {
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

    void inspect() override { 

        ImGui::InputFloat3("Position", &position.x);
    }

    COMPONENT_DEFINE_BODY();
};

}  // namespace Components
}  // namespace Blazar
