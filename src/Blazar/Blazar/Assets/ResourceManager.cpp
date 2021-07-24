#include "bzpch.h"

#include <filesystem>
#include "ResourceManager.h"

namespace Blazar {

ResourceManager* g_resourceManager;

ResourceManager::ResourceManager() { m_vfs = new VFS::VFS(); }

bool ResourceManager::ReadFileFromPath(std::string_view path, std::vector<std::byte>& outBuffer) {
    ZoneScoped;

    if (m_vfs->exists(std::string(path))) {
        std::vector<std::byte>& out = m_vfs->read(std::string(path));
        outBuffer = out;
        return true;
    } else {
        return false;
    }
}

ResourceManager* ResourceManager::Get() {
    if (g_resourceManager == nullptr) { g_resourceManager = new ResourceManager(); }
    return g_resourceManager;
}

}  // namespace Blazar
