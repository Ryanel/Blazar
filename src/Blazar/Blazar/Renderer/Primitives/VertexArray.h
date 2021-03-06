#pragma once

namespace Blazar {

class VertexBuffer;
class IndexBuffer;

/// A vertex array, which binds layout properties with buffers.
class VertexArray {
   public:
    virtual ~VertexArray() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;

    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

    // Factory method
    static Ref<VertexArray> Create();
    static Ref<VertexArray> Create(const Ref<VertexBuffer> vertBuffer, Ref<IndexBuffer> indexBuffer);
};

}  // namespace Blazar
