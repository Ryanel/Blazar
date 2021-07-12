#include "bzpch.h"

#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Renderer/Renderer.h"

#include "Blazar/Platform/OpenGL/OpenGLTexture.h"

#include "Blazar/Assets/Resource.h"

namespace Blazar {

Ref<Texture> Texture2D::Create(const std::string& path, const TextureProperties& properties) {
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

template <>
Texture2D* DeserializeToResource(std::string_view path, std::vector<char>& data) {
    TextureProperties properties;
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return OpenGLTexture2D::FromData(data, properties);
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }
    return nullptr;
}

}  // namespace Blazar
