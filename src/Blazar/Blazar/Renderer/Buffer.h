#pragma once

namespace Blazar {

class VertexBuffer {
   public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const= 0;
    virtual void Unbind() const = 0;

    // Factory method
    static VertexBuffer* Create(float* verticies, size_t size);
};

class IndexBuffer {
   public:
    virtual ~IndexBuffer() {}
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    // Factory method
    static IndexBuffer* Create(uint32_t* verticies, size_t size);
};

};  // namespace Blazar
