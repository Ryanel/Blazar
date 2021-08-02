#include "bzpch.h"

#include <filesystem>
#include "ResourceManager.h"

namespace Blazar {

ResourceManager* g_resourceManager;

ResourceManager::ResourceManager() { m_vfs = new VFS::VFS(); }
bool ResourceManager::exists(std::string path) { return m_entries.find(path) != m_entries.end(); }
void ResourceManager::unload(std::string path) { 
    if (exists(path)) {
        m_entries.erase(path);
    } else {
        LOG_CORE_ERROR("Resource Manager: Attempted to clear a path that doesn't exist ({})", path);
    }
    m_entries.erase(path);

}
void ResourceManager::clean() {
    // Unload any resources that have use_count = 1
    std::list<std::string> toClean;
    int clean_stats = 0;
    for (auto& it : m_entries) {
        if (it.second.use_count() <= 1) { toClean.push_back(it.first); }
    }

    for (auto& it : toClean) { m_entries.erase(it); }
}
bool ResourceManager::vfs_read(std::string_view path, std::vector<std::byte>& outBuffer) {
    ZoneScoped;
    if (m_vfs->exists(std::string(path))) {
        std::vector<std::byte>& out = m_vfs->read(std::string(path));
        outBuffer = out;
        return true;
    }
    return false;
}

ResourceManager* ResourceManager::get() {
    if (g_resourceManager == nullptr) { g_resourceManager = new ResourceManager(); }
    return g_resourceManager;
}

}  // namespace Blazar
