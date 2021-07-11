#include "bzpch.h"

#include "Blazar/Renderer/RenderTexture.h"
#include "Blazar/Renderer/Renderer.h"

#include "Blazar/Platform/OpenGL/OpenGLRenderTexture.h"

#include "Blazar/Assets/Resource.h"

namespace Blazar {

Ref<RenderTexture> RenderTexture::Create(const RenderTextureProperties& properties) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLRenderTexture>(properties);
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

}  // namespace Blazar
