#pragma once

#include <vector>

#include "Blazar/Memory.h"
#include "Buffer.h"
#include "ShaderDataType.h"
#include "VertexArray.h"

namespace Blazar {
class Quad {
   public:
    // Create a fullscreen quad to render the game to.
    BufferLayout quad_layout = {
        {ShaderDataType::Float3, "a_Position"},  // 00: Position
        {ShaderDataType::Float2, "a_TexCoord"},  // 12: Color
    };

    float quad_verts[5 * 4] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // v0
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,  // v1
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  // v2,
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // v3,
    };

    uint32_t          quad_indicies[6] = {0, 1, 2, 2, 3, 0};
    Ref<VertexBuffer> vbo;
    Ref<IndexBuffer>  ibo;
    Ref<VertexArray>  vao;

    Quad() {
        vbo = VertexBuffer::Create(quad_verts, sizeof(quad_verts), quad_layout);
        ibo = IndexBuffer::Create(quad_indicies, sizeof(quad_indicies));
        vao = VertexArray::Create(vbo, ibo);
    }
};

};  // namespace Blazar
