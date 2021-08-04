#pragma once

#include <vector>

#include "Blazar/Memory.h"
#include "Buffer.h"
#include "ShaderDataType.h"
#include "VertexArray.h"

namespace Blazar {
class Mesh {
   public:
    Ref<Mesh> LoadMesh(std::stringbuf vpath);

    Ref<VertexArray> vao;
    Ref<VertexBuffer> vbo;
    Ref<IndexBuffer> index;
};

};  // namespace Blazar
