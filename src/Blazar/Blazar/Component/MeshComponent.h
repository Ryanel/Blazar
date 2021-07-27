#pragma once

#include "Blazar/Memory.h"
#include "Blazar/Renderer/Primitives/VertexArray.h"

namespace Blazar {
namespace Components {

/// Reference to a mesh
struct MeshComponent {
    Ref<VertexArray> mesh;
};

}  // namespace Components
}  // namespace Blazar
