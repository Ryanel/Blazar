#pragma once

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include "Blazar/Memory.h"
#include "Blazar/Renderer/Primitives/Texture.h"
#include "Blazar/VFS/VFS.h"
#include "Resource.h"
#include "Tracy.hpp"

namespace Blazar {

/// Loads resources from disk
class ResourceManager {
   protected:
    ResourceManager();  ///< Constructor

   public:
    /// Attempts to get a resource that has already been loaded. If it hasn't been loaded, this causes an error.
    template<class T> Ref<Resource<T>> Load(std::string path) {
        ZoneScoped;
        auto& it = m_entries.find(path);
        if (it != m_entries.end()) {
            if (!it->second->loaded()) {
                LOG_CORE_ERROR("Resource {} was not loaded, even though it exists.", path);
                throw;
            }

            auto& nptr = std::static_pointer_cast<Resource<Texture2D>>(it->second);
            return nptr;
        }

        LOG_CORE_ERROR("Resource {} has not been created yet, and cannot be loaded.", path);
        throw;
    }

    /// Add a resource to the Resource Manager to be cached
    template<class T> bool Add(std::string path, Ref<Resource<T>> res) {
        m_entries.emplace(path, std::static_pointer_cast<IResource>(res));
        return true;
    }

    bool exists(std::string path);    ///< Checks if path exists as a Resource, not if it exists on the filesystem
    void unload(std::string path);    ///< Attempts to unload the resource. Note: Does not destroy any other references,
                                      ///< so the object may live past this.
    void                    clean();  ///< Attempts to garbage collect any items with no references.
    static ResourceManager* get();    ///< Gets the resource manager instance

    /// Reads a file's data from the VFS. Returns true if successful.
    bool vfs_read(std::string_view path, std::vector<std::byte>& outBuffer);

    VFS::VFS* m_vfs;  ///< The virtual filesystem that the resource manager uses to load resources.
   private:
    std::unordered_map<std::string, Ref<IResource>> m_entries;
};
}  // namespace Blazar
