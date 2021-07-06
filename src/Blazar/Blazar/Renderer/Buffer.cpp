#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLBuffer.h"
#include "Blazar/Renderer/Buffer.h"
#include "Blazar/Renderer/Renderer.h"

namespace Blazar {

Ref<VertexBuffer> VertexBuffer::Create(float* verticies, size_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return Ref<VertexBuffer>(new OpenGLVertexBuffer(verticies, size));
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* verticies, size_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return Ref<IndexBuffer>(new OpenGLIndexBuffer(verticies, size));
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

}  // namespace Blazar
