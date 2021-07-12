#pragma once

#include <cstdint>
#include "Blazar/Renderer/Primitives/Buffer.h"

namespace Blazar {

class OpenGLVertexBuffer : public VertexBuffer {
   public:
    OpenGLVertexBuffer(float* verticies, size_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;


    virtual void SetLayout(const BufferLayout& layout);
    virtual const BufferLayout& GetLayout() const;


   private:
    uint32_t m_Id;
    BufferLayout m_Layout;
};


class OpenGLIndexBuffer : public IndexBuffer {
   public:
    OpenGLIndexBuffer(uint32_t* verticies, size_t size);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual size_t GetCount() const override { return m_Count; }
   private:
    uint32_t m_Id;
    size_t m_Count;
};

};  // namespace Blazar
