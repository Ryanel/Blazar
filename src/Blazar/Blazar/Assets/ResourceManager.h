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
    template<class T> Ref<Resource<T>> Load(std::string path) {
        auto& it = m_entries.find(path);
        if (it != m_entries.end()) {
            if (!it->second->Loaded()) {
                LOG_CORE_ERROR("Resource {} was not loaded, even though it exists.", path);
                throw;
            }

            auto& nptr = std::static_pointer_cast<Resource<Texture2D>>(it->second);
            return nptr;
        }

        LOG_CORE_ERROR("Resource {} has not been created yet, and cannot be loaded.", path);
        throw;
    }

    template<class T> bool Add(std::string path, Ref<Resource<T>> res) {
        m_entries.emplace(path, std::static_pointer_cast<IResource>(res));
        return true;
    }

    bool Exists(std::string path);
    void Unload(std::string path);
    void Clean();
    bool ReadFromVFS(std::string_view path, std::vector<std::byte>& outBuffer);

    static ResourceManager* Get();

    VFS::VFS* m_vfs;

   private:
    std::unordered_map<std::string, Ref<IResource>> m_entries;
};
}  // namespace Blazar
