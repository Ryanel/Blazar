#include "bzpch.h"

#include "Blazar/Renderer/Texture.h"
#include "Blazar/Renderer/Renderer.h"

#include "Blazar/Platform/OpenGL/OpenGLTexture.h"

namespace Blazar {

Ref<Texture> Texture2D::Create(const std::string& path, TextureProperties& properties) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path, properties);
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
}

}  // namespace Blazar
