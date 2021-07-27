#pragma once

#include <glm/glm.hpp>

namespace Blazar {
namespace Components {
/// A RenderTransform is a transform relative to the current camera. The reason this is not just a normal transform is
/// due to the desire to allow for alternative coordinate schemes in the game code.
struct RenderTransform {
    glm::vec3 position;  ///< The posititon of this entity, relative to the camera.
};
}  // namespace Components
}  // namespace Blazar
