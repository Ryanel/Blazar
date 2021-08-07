#pragma once

#include "Blazar/Component/IComponent.h"
#include "Blazar/Memory.h"
#include "Blazar/Renderer/Primitives/VertexArray.h"

namespace Blazar {
namespace Components {

/// Reference to a mesh
struct MeshComponent : IComponent {
    Ref<VertexArray> mesh;

    MeshComponent(Ref<VertexArray> mesh) : mesh(mesh) {}
    COMPONENT_DEFINE_BODY();
};

}  // namespace Components
}  // namespace Blazar
