#include "bzpch.h"

#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "VertexArray.h"


namespace Blazar {

VertexArray* VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

}  // namespace Blazar