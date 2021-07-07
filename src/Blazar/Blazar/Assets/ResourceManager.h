#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "Resource.h"

namespace Blazar {

// Loads and caches resources from disk.
class ResourceManager {
   public:
    ResourceManager() : m_loadedResources() {}
    bool GetBinaryFromDisk(std::string_view path, std::vector<char>& outBuffer);
    // void LoadFromArchive();

   public:
    template <class T>
    std::optional<Resource<T>> Load(std::string path) {
        std::vector<char> binaryData;
        // Determine where this resource is
        if (GetBinaryFromDisk(path, binaryData)) {
            auto result = DeserializeToResource<T>(path, binaryData);
            if (result != nullptr) { return std::optional<Resource<T>>(Resource<T>(path, result)); }
        }
        return std::nullopt;
    }

   private:
    std::unordered_map<std::string, ResourceBase> m_loadedResources;
};
}  // namespace Blazar
