#pragma once

#include <cstdint>
#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/VertexArray.h"

namespace Blazar {

class OpenGLVertexArray : public VertexArray {
   public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(Ref<IndexBuffer>& indexBuffer) override;

    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const;
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const;

   private:
    uint32_t m_Id;
    BufferLayout m_Layout;
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};
};  // namespace Blazar
