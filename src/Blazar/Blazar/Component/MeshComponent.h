#pragma once

#include "Blazar/Component/IComponent.h"
#include "Blazar/Memory.h"
#include "Blazar/Renderer/Primitives/VertexArray.h"

namespace Blazar {
namespace Components {

/// Reference to a mesh
struct MeshComponent {
    Ref<VertexArray> mesh;
    MeshComponent(Ref<VertexArray> mesh) : mesh(mesh) {}

    REFLECTION_REFLECT(MeshComponent);
    REFLECTION_DESCRIPTION("Holds a reference to a mesh");
    REFLECTION_MEMBER("mesh");
    REFLECTION_END();
};

}  // namespace Components
}  // namespace Blazar
