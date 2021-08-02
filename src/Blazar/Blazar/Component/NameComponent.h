#pragma once

#include <string>

namespace Blazar {
namespace Components {

struct NameComponent {
    NameComponent(std::string name) : name(name) {}
    std::string name;  ///< The name of this entity
};

}  // namespace Components
}  // namespace Blazar
