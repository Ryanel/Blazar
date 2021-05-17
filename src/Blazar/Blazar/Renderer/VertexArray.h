#pragma once

#include "Blazar/Renderer/Buffer.h"

namespace Blazar {

/// A vertex array, which binds layout properties with buffers.
class VertexArray {
   public:
    virtual ~VertexArray() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
    // Factory method
    static VertexArray* Create();
};

}  // namespace Blazar
