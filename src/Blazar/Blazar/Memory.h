#pragma once

#include <memory>

namespace Blazar {
    template<typename T>
    using Scope = std::unique_ptr<T>; ///< A Scoped Pointer, destroyed at end of scope via RAII

    template <typename T>
    using Ref = std::shared_ptr<T>; ///< A Reference Counted pointer, destroyed when reference count is 0
}