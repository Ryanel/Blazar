#include "bzpch.h"

#include "VertexArray.h"
#include "Buffer.h"

#include "Blazar/Renderer/Renderer.h"

#include "Blazar/Platform/OpenGL/OpenGLBuffer.h"
#include "Blazar/Platform/OpenGL/OpenGLVertexArray.h"

namespace Blazar {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::api()) {
        case RendererAPI::API::OpenGL:
            return Ref<VertexArray>(new OpenGLVertexArray());
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

Ref<VertexArray> VertexArray::Create(const Ref<VertexBuffer> vertBuffer, Ref<IndexBuffer> indexBuffer) {
    auto vtxArray = Ref<VertexArray>();
    switch (Renderer::api()) {
        case RendererAPI::API::OpenGL:
            vtxArray.reset(new OpenGLVertexArray());
            vtxArray->AddVertexBuffer(vertBuffer);
            vtxArray->SetIndexBuffer(indexBuffer);
            return vtxArray;
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

}  // namespace Blazar
