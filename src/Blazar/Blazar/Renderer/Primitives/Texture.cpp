#include "bzpch.h"

#include "Blazar/Assets/Resource.h"
#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Platform/OpenGL/OpenGLTexture.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/Renderer/Renderer.h"

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

Ref<Resource<Texture2D>> Texture2D::Load(const std::string& path, TextureProperties props) {
    auto* rm = ResourceManager::Get();

    // Check if the resource already exists. If so, load it
    if (rm->Exists(path)) { return rm->Load<Texture2D>(path); }

    // Check if the filesystem has such a path. If not, throw.
    if (!rm->m_vfs->exists(path)) { throw; }

    // Attempt to load the data.
    std::vector<std::byte> data;
    if (!rm->ReadFromVFS(path, data)) {
        LOG_CORE_ERROR("Reading from VFS failed");
        throw;
    }

    Ref<Resource<Texture2D>> ptr = std::make_shared<Resource<Texture2D>>();
    Texture2D* tex_ptr = nullptr;
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            tex_ptr = OpenGLTexture2D::FromData(data, props);
            break;
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            throw;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            throw;
    }

    ptr->set_data(tex_ptr);
    rm->Add<Texture2D>(path, ptr);

    return ptr;
}

/*
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
*/

}  // namespace Blazar
