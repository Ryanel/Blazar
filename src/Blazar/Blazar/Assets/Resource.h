#pragma once

#include <memory>
#include <string>

#include "Blazar/Log.h"
#include "Blazar/Renderer/Primitives/Texture.h"

namespace Blazar {

/// Base Resource Class
class ResourceBase {
   protected:
    std::string m_path;

   public:
    ResourceBase(std::string path) : m_path(path) {}
    virtual bool Loaded() = 0;
    std::string_view GetPath() { return m_path; }
};

/// A loaded resource
template<typename T> class Resource : public ResourceBase {
   public:
    Resource() : ResourceBase("") {}  ///< Cannot be loaded, will error if trying to load
    Resource(std::string path) : ResourceBase(path) {}
    bool Loaded() override { return m_data.get() != nullptr; }
    void Load() {
        LOG_CORE_ERROR("Attempted to load resource {}, which has no load function. Unable to load.", m_path);
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

#define RESOURCE_LOAD_FUNCTION(type)                                                                              \
    type* LoadResource(Resource<type>& resource);                                                                 \
    template<> void Resource<type>::Load() {                                                                      \
        type* r = LoadResource(*this);                                                                            \
        if (r != nullptr) {                                                                                       \
            m_data.reset(r);                                                                                      \
            LOG_CORE_TRACE("Loaded resource {}", m_path);                                                         \
        } else {                                                                                                  \
            LOG_CORE_ERROR("Failed to load resource {}, load function failed (usually, file not found)", m_path); \
            m_data = nullptr;                                                                                     \
        }                                                                                                         \
    }

RESOURCE_LOAD_FUNCTION(Texture2D)

}  // namespace Blazar
