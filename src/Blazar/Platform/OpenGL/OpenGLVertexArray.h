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

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) override;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const;
    virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

   private:
    uint32_t m_Id;
    BufferLayout m_Layout;
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};
};  // namespace Blazar
