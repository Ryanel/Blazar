#include "bzpch.h"

#include "Blazar/Renderer/Primitives/Buffer.h"
#include "Blazar/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Blazar {

// ----------------------------------------------------
// Vertex Buffer
// ----------------------------------------------------

OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, size_t size) {
    glCreateBuffers(1, &m_Id);
    glBindBuffer(GL_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_Id); }
void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_Id); }
void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) { m_Layout = layout; }
const BufferLayout& OpenGLVertexBuffer::GetLayout() const { return m_Layout; }

// ----------------------------------------------------
// Index Buffer
// ----------------------------------------------------

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, size_t count) : m_Count(count)  {
    glCreateBuffers(1, &m_Id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_Id); }
void OpenGLIndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id); }
void OpenGLIndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

};  // namespace Blazar
