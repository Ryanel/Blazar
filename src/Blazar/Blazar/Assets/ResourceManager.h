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

class AssetEditorWindow;
namespace Blazar {

#define RESMAN_DEFINE_LOAD(typen, comparison)                                           \
    template<> Ref<Resource<typen>> Load(std::string path) {                        \
        ZoneScoped;                                                                     \
        auto& it = m_entries.find(path);                                                \
        if (it != m_entries.end()) {                                                    \
            if (!it->second->Loaded()) { throw; }                                       \
            auto& nptr = std::dynamic_pointer_cast<Resource<typen>>(it->second);    \
            return nptr;                                                                \
        } else {                                                                        \
            Ref<Resource<typen>> ptr = std::make_shared<Resource<typen>>(path); \
            ptr->Load();                                                                \
            m_entries.emplace(path, ptr);                                               \
            return ptr;                                                                 \
        }                                                                               \
        return nullptr;                                                                 \
    }

/// Loads resources from disk
class ResourceManager {
   protected:
    ResourceManager();  ///< Constructor

   public:
    bool ReadFromVFS(std::string_view path, std::vector<std::byte>& outBuffer);  ///< Loads a binary from the disk

    static ResourceManager* Get();  ///< Returns the current Resource Manager

    template<class T> Ref<Resource<T>> Load(std::string path) { return nullptr; }

    Ref<Resource<Texture2D>> LoadTexture(std::string path, TextureProperties props = TextureProperties()) {
        ZoneScoped;
        auto& it = m_entries.find(path);
        if (it != m_entries.end()) {
            if (!it->second->Loaded()) { throw; }
            auto& nptr = std::dynamic_pointer_cast<Resource<Texture2D>>(it->second);
            return nptr;
        } else {
            Ref<Resource<Texture2D>> ptr = std::make_shared<Resource<Texture2D>>(path);
            ptr->Load(props);
            m_entries.emplace(path, ptr);
            return ptr;
        }
        return nullptr;
    }

    bool Loaded(std::string path) {
        return m_entries.find(path) != m_entries.end();
    }

    void Unload(std::string path) {
        auto& it = m_entries.find(path);
        if (it != m_entries.end()) { m_entries.erase(path); }
    }

    void Clean() {
        // Unload any resources that have use_count = 1

        std::list<std::string> toClean;

        int clean_stats = 0;
        for (auto& it : m_entries) {
            if (it.second.use_count() <= 1) { toClean.push_back(it.first); }
        }

        for (auto& it : toClean) { m_entries.erase(it); }
    }

    VFS::VFS* m_vfs;

   private:
    std::unordered_map<std::string, Ref<ResourceBase>> m_entries;
    friend class AssetEditorWindow;
};
}  // namespace Blazar
