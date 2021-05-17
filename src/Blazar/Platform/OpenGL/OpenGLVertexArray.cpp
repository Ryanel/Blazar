#include "bzpch.h"

#include <glad/glad.h>
#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/ShaderDataType.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Blazar {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
    }
}

OpenGLVertexArray::OpenGLVertexArray() { glCreateVertexArrays(1, &m_Id); }
OpenGLVertexArray::~OpenGLVertexArray() {}
void OpenGLVertexArray::Bind() const { glBindVertexArray(m_Id); }
void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }
void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
    BLAZAR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no associated layout!");

    glBindVertexArray(m_Id);
    vertexBuffer->Bind();

    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout.GetElements()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetElementCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}
void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_Id);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}
const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const {
    return m_VertexBuffers;
}
const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const { return m_IndexBuffer; }
}  // namespace Blazar
