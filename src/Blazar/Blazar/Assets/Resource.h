#pragma once

#include <string>

#include "Blazar/Config.h"

namespace Blazar {

// A resource is a loaded "thing" in Blazar. Resources are passed around, and act like shared_ptrs
class ResourceBase {
    class RC {
       public:
        void Lock() { m_count++; }
        void Release() { m_count--; }
        int Get() { return m_count; }

       private:
        int m_count = 0;
    };

   protected:
    // Constructor
    ResourceBase(const std::string& path) : m_path(path), m_size(0), m_refcount(new RC()) {}

    ResourceBase(const std::string& path, size_t size, RC* rc) : m_path(path), m_size(size), m_refcount(rc) {}

   public:
    ResourceBase() : m_path(""), m_size(0), m_refcount(nullptr) {}

    // Copy
    ResourceBase(const ResourceBase& other)
        : m_path(other.m_path), m_size(other.m_size), m_refcount(other.m_refcount) {}

    virtual std::string_view getPath() { return m_path; }

    // Destructor
    virtual ~ResourceBase() {}

   protected:
    RC* m_refcount;
    std::string m_path;
    size_t m_size;
};

template <typename T>
class Resource : public ResourceBase {
    template <typename T>
    class ResourcePointer {
       public:
        ResourcePointer(T* d) : data(d) {}
        T* data;
    };

   public:
    Resource() : ResourceBase("", 0, nullptr), m_data(nullptr) {}

    // Constructor
    Resource(const std::string& path, T* data) : ResourceBase(path), m_data(new ResourcePointer(data)) {
#ifdef BLAZAR_CONFIG_LOG_RESOURCE_USAGE
        LOG_CORE_TRACE("[Resource] Created: {}", m_path);
#endif
        m_refcount->Lock();
    }

    // Copy
    Resource(const Resource& res) : ResourceBase(res), m_data(res.m_data) {
        m_refcount->Lock();
#ifdef BLAZAR_CONFIG_LOG_RESOURCE_USAGE
        LOG_CORE_TRACE("[Resource] Copied {}, ref: {} +", m_path, m_refcount->Get());
#endif
    }

    // Move
    Resource(Resource&& other) : ResourceBase(other.m_path, other.m_size, other.m_refcount), m_data(other.m_data) {
#ifdef BLAZAR_CONFIG_LOG_RESOURCE_USAGE
        LOG_CORE_TRACE("[Resource] Moved {}, ref: {} =", m_path, m_refcount->Get());
#endif
        other.m_path = "";
        other.m_size = 0;
        other.m_refcount = nullptr;
        other.m_data = nullptr;
    }

    Resource& operator=(Resource&& other) {
        if (this != &other) {
            if (m_refcount) { delete m_refcount; }
            if (m_data) { delete m_data; }

            m_size = other.m_size;
            m_refcount = other.m_refcount;
            m_path = other.m_path;
            m_data = other.m_data;

            other.m_path = "";
            other.m_size = 0;
            other.m_refcount = nullptr;
            other.m_data = nullptr;
        }
        return *this;
    }

    // Deconstructor
    ~Resource() {
        if (m_refcount == nullptr) {
            // Empty object, return
            return;
        }

        m_refcount->Release();

        if (m_refcount->Get() == 0) {
#ifdef BLAZAR_CONFIG_LOG_RESOURCE_USAGE
            LOG_CORE_TRACE("[Resource] Destroyed {}, no more references", m_path);
#endif
            if (m_data->data != nullptr) { delete m_data->data; }
            delete m_data;
            delete m_refcount;
        } else {
#ifdef BLAZAR_CONFIG_LOG_RESOURCE_USAGE
            LOG_CORE_TRACE("[Resource] {}, ref: {} -", m_path, m_refcount->Get());
#endif
        }
    }

    T& get() { return *m_data->data; }
    T* operator->() const { return m_data->data; }
    T& operator*() const { return *m_data->data; }

    int refs() const { return m_refcount->Get(); }

   private:
    ResourcePointer<T>* m_data;
};

// Deserializes a resource into a T from binary data.
template <class T>
T* DeserializeToResource(std::string_view path, std::vector<char>& data);
}  // namespace Blazar
