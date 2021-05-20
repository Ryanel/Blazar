#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Blazar {

class Shader {
   public:
    // Factory methods
    static Shader * FromFile(std::string path);
    static Shader * FromText(std::string vertex, std::string fragment);

    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};

};  // namespace Blazar
