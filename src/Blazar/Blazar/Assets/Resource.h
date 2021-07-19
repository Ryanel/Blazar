#pragma once

#include <string>

#include "Blazar/Config.h"

namespace Blazar {

/// A resource is a loaded "thing" in Blazar. Resources are passed around, and act like shared_ptrs
class ResourceBase {
    /// Reference counting
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
    ResourceBase(const std::string& path) : m_path(path), m_size(0), m_rc(new RC()) {}
    ResourceBase(const std::string& path, size_t size, RC* rc) : m_path(path), m_size(size), m_rc(rc) {}

   public:
    ResourceBase() : m_path(""), m_size(0), m_rc(nullptr) {}

    // Copy
    ResourceBase(const ResourceBase& other) : m_path(other.m_path), m_size(other.m_size), m_rc(other.m_rc) {}

    virtual std::string_view path() { return m_path; }

    // Destructor
    virtual ~ResourceBase() {}

   protected:
    RC* m_rc;            ///< Resource Counting
    std::string m_path;  ///< Path of this resource
    size_t m_size;       ///< Size, in bytes, of the resource
};

/// A reference counted resource. Stores path information.
template <typename T>
class Resource : public ResourceBase {
   public:
    Resource() : ResourceBase("", 0, nullptr), m_data(nullptr) {}
    Resource(const std::string& path, T* data) : ResourceBase(path), m_data(data) { m_rc->Lock(); }
    Resource(const Resource& res) : ResourceBase(res), m_data(res.m_data) { m_rc->Lock(); }

    // Move
    Resource(Resource&& other) : ResourceBase(other.m_path, other.m_size, other.m_rc), m_data(other.m_data) {
        other.m_path = "";
        other.m_size = 0;
        other.m_rc = nullptr;
        other.m_data = nullptr;
    }

    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            if (m_rc) { delete m_rc; }
            if (m_data) { delete m_data; }

            m_size = other.m_size;
            m_rc = other.m_rc;
            m_path = other.m_path;
            m_data = other.m_data;

            other.m_path = "";
            other.m_size = 0;
            other.m_rc = nullptr;
            other.m_data = nullptr;
        }
        return *this;
    }

    // Deconstructor
    ~Resource() {
        // Empty object, return
        if (m_rc == nullptr) { return; }

        m_rc->Release();

        if (m_rc->Get() == 0) {
            if (m_data != nullptr) { delete m_data; }
            delete m_data;
            delete m_rc;
        }
    }

    T& get() { return *m_data; }
    T* operator->() const { return m_data; }
    T& operator*() const { return *m_data; }

    int refs() const { return m_rc->Get(); }

   private:
    T* m_data;
};

// Deserializes a resource into a T from binary data.
template <class T>
T* DeserializeToResource(std::string_view path, std::vector<char>& data);
}  // namespace Blazar
