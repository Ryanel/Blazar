#pragma once

#include "Blazar/Log.h"

namespace Blazar {

/// Base Resource Class
class IResource {
   public:
    IResource() {}
    virtual ~IResource() {}
    virtual bool Loaded() = 0;
};

/// A loaded resource
template<class T> class Resource : public IResource {
   public:
    Resource() : IResource() {}
    virtual ~Resource() { 
        LOG_CORE_TRACE("Resource destroyed (no references)");
        delete m_data;
    }
    T* data() {
        if (m_data == nullptr) { throw; }
        return m_data;
    }

    void set_data(T* data) { m_data = data; }
    virtual bool Loaded() { return m_data != nullptr; }

   private:
    T* m_data = nullptr;
};
}  // namespace Blazar
