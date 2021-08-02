#pragma once

#include "Blazar/Log.h"

namespace Blazar {

/// Base Resource Class
class IResource {
   public:
    IResource() {}
    virtual ~IResource() {}
    virtual bool loaded() = 0;
};

/// A loaded resource
template<class T> class Resource : public IResource {
   public:
    /// Constructs the resource
    Resource() : IResource() {}

    /// Deletes the resource, freeing the held data.
    /// Normally not called until the Ref has no more references
    virtual ~Resource() {
        LOG_CORE_TRACE("Resource destroyed (no references)");
        delete m_data;
    }

    /// Retrieves the data. Data is garentueed to not be null.
    T* data() {
        if (m_data == nullptr) { throw; }
        return m_data;
    }

    /// Sets the data
    void set_data(T* data) { m_data = data; }

    /// Returns true if data has been inserted already.
    virtual bool loaded() { return m_data != nullptr; }

   private:
    T* m_data = nullptr;
};
}  // namespace Blazar
