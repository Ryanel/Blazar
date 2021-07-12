#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Blazar/Core.h"

namespace Blazar {

class Shader {
   public:
    // Factory methods
    static Ref<Shader> FromFile(std::string path);
    static Ref<Shader> FromText(std::string vertex, std::string fragment);

    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    void SetName(std::string n) { name = n; }
    std::string name;
};

};  // namespace Blazar
