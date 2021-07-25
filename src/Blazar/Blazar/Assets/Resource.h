#pragma once

#include <memory>
#include <string>
#include "Blazar/Log.h"
#include "Tracy.hpp"

#include "Blazar/Renderer/Primitives/Texture.h"

namespace Blazar {

// Types of Resources
enum class ResourceType : int { Unknown, Tex2D };

/// Base Resource Class
class ResourceBase {
   protected:
    std::string m_path;
    ResourceType m_type;

   public:
    ResourceBase(std::string path, ResourceType type) : m_path(path), m_type(type) {}
    virtual bool Loaded() = 0;
    std::string_view GetPath() { return m_path; }
    ResourceType Type() { return m_type; }
};

/// A loaded resource
template<typename T> class Resource : public ResourceBase {
   public:
    Resource() : ResourceBase("", ResourceType::Unknown) {}  ///< Cannot be loaded, will error if trying to load
    Resource(std::string path) : ResourceBase(path, ResourceType::Unknown) {}
    ~Resource() { LOG_CORE_WARN("Destroyed resource {}", m_path); }
    bool Loaded() override { return m_data.get() != nullptr; }

    Resource<T>* Load() {
        LOG_CORE_ERROR("Attempted to load resource {}, which has no load function. Unable to load.", m_path);
        return *this;
    }
    Resource<T>* Load(TextureProperties props) {
        LOG_CORE_ERROR("Attempted to load resource {}, which has no load function. Unable to load.", m_path);
        return *this;
    }

    std::shared_ptr<T> data() {
#ifdef BLAZAR_CFG_RESOURCE_LAZYLOAD
        if (!Loaded()) { Load(); }
#endif
        if (!Loaded()) { throw; }
        return m_data;
    }

   private:
    std::shared_ptr<T> m_data;
};

#define RESOURCE_SETUP(type, restype)                                                                             \
    type* LoadResource(Resource<type>& resource);                                                                 \
    template<> Resource<type>::Resource(std::string path) : ResourceBase(path, restype) {}                        \
    template<> void Resource<type>::Load() {                                                                      \
        ZoneScoped;                                                                                               \
        if (Loaded()) { return; }                                                                                 \
        type* r = LoadResource(*this);                                                                            \
        if (r != nullptr) {                                                                                       \
            m_data.reset(r);                                                                                      \
            LOG_CORE_TRACE("Loaded resource {}", m_path);                                                         \
        } else {                                                                                                  \
            LOG_CORE_ERROR("Failed to load resource {}, load function failed (usually, file not found)", m_path); \
            m_data = nullptr;                                                                                     \
        }                                                                                                         \
    }

// Forward includes
class Texture2D;

Texture2D* LoadResource(Resource<Texture2D>& resource, TextureProperties props);
template<> Resource<Texture2D>::Resource(std::string path) : ResourceBase(path, ResourceType::Tex2D) {}
template<> Resource<Texture2D>* Resource<Texture2D>::Load() {
    ZoneScoped;
    if (Loaded()) { return this; }
    Texture2D* r = LoadResource(*this, TextureProperties());
    if (r != nullptr) {
        m_data.reset(r);
        LOG_CORE_TRACE("Loaded resource {}", m_path);
    } else {
        LOG_CORE_ERROR("Failed to load resource {}, load function failed (usually, file not found)", m_path);
        m_data = nullptr;
    }
    return this;
}

template<> Resource<Texture2D>* Resource<Texture2D>::Load(TextureProperties props) {
    ZoneScoped;
    if (Loaded()) { return this; }
    Texture2D* r = LoadResource(*this, props);
    if (r != nullptr) {
        m_data.reset(r);
        LOG_CORE_TRACE("Loaded resource {}", m_path);
    } else {
        LOG_CORE_ERROR("Failed to load resource {}, load function failed (usually, file not found)", m_path);
        m_data = nullptr;
    }
    return this;
}

// RESOURCE_SETUP(Texture2D, ResourceType::Tex2D)

}  // namespace Blazar
