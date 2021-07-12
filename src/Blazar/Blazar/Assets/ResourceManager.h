#pragma once

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include "Resource.h"

namespace Blazar {

// Loads and caches resources from disk.
class ResourceManager {
   protected:
    ResourceManager() : m_loadedResources() {}
    bool GetBinaryFromDisk(std::string_view path, std::vector<char>& outBuffer);
    // void LoadFromArchive();

   public:
    static ResourceManager* Get();

   public:
    template <class T>
    std::optional<Resource<T>> Load(std::string path, bool required = false) {
        ZoneScoped;
        LOG_CORE_TRACE("[Res Man]: Attempting to load {}", path);

        // TODO: We lock here to avoid bugs right now, but this needs to be Optimized!
        std::lock_guard<std::mutex> lock(m_loadedResourceLock);
        // Check cache
        {
            ZoneScopedN("Check Cache");

            std::unordered_map<std::string, ResourceBase*>::const_iterator cached_item = m_loadedResources.find(path);

            if (cached_item != m_loadedResources.end()) {
                LOG_CORE_TRACE("[Res Man]: Found in cache");
                Resource<T>* resource = static_cast<Resource<T>*>(cached_item->second);
                return std::optional<Resource<T>>(*resource);
            }
        }

        {
            ZoneScopedN("Check Disk");
            LOG_CORE_TRACE("[Res Man]: Not in cache, checking FS");
            std::vector<char> binaryData;
            // Determine where this resource is
            if (GetBinaryFromDisk(path, binaryData)) {
                auto result = DeserializeToResource<T>(path, binaryData);
                if (result != nullptr) {
                    auto* resource = new Resource<T>(path, result);
                    m_loadedResources[path] = resource;  // Cache
                    return std::optional<Resource<T>>(*resource);
                }
            }
        }

        if (required == true) { throw; }
        return std::nullopt;
    }

    void Unload(std::string path);

   private:
    std::unordered_map<std::string, ResourceBase*> m_loadedResources;
    std::mutex m_loadedResourceLock;
};
}  // namespace Blazar
