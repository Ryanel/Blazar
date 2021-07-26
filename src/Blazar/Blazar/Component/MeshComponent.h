#pragma once

#include <glm/glm.hpp>
#include "Blazar/Renderer/Primitives/VertexArray.h"
#include "Blazar/Memory.h"

namespace Blazar {

struct MeshComponent {
	Ref<VertexArray> mesh;
};

}  // namespace Blazar
