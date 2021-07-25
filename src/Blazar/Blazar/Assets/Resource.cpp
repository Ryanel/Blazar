#include "bzpch.h"

#include "Blazar/Platform/OpenGL/OpenGLTexture.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Renderer/Renderer.h"
#include "Resource.h"
#include "ResourceManager.h"

namespace Blazar {

    /*
Texture2D* LoadResource(Resource<Texture2D>& resource, TextureProperties properties) {
    ZoneScopedN("Load Resource (Specific)");
    std::vector<std::byte> data;
    if (ResourceManager::Get()->ReadFromVFS(resource.path(), data)) {
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
    } else {
        LOG_CORE_INFO("Failed to load Texture {}!", resource.path());
    }

    return nullptr;
}
*/
}  // namespace Blazar
