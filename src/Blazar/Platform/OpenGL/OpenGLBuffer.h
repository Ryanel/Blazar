#pragma once

#include <cstdint>
#include "Blazar/Renderer/Buffer.h"

namespace Blazar {

class OpenGLVertexBuffer : public VertexBuffer {
   public:
    OpenGLVertexBuffer(float* verticies, size_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

   private:
    uint32_t m_Id;
};


class OpenGLIndexBuffer : public IndexBuffer {
   public:
    OpenGLIndexBuffer(uint32_t* verticies, size_t size);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

   private:
    uint32_t m_Id;
};

};  // namespace Blazar
