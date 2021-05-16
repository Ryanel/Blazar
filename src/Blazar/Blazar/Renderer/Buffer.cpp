#include "bzpch.h"

#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Blazar {

VertexBuffer* VertexBuffer::Create(float* verticies, size_t size) {
    switch (Renderer::CurrentAPI()) {
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(verticies, size);
        case RendererAPI::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

IndexBuffer* IndexBuffer::Create(uint32_t* verticies, size_t size) {
    switch (Renderer::CurrentAPI()) {
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(verticies, size);
        case RendererAPI::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

}  // namespace Blazar
