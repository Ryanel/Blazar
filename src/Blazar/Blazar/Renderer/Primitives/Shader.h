#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Blazar/Core.h"

namespace Blazar {

class Shader {
   public:
    // Factory methods
    static Ref<Shader> Load(std::string path);
    static Ref<Shader> FromText(std::string vertex, std::string fragment);

    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    void SetName(std::string n) { name = n; }
    std::string name;

    virtual void SetMat4(const std::string& str, const glm::mat4& mat) = 0;
    virtual void SetInt(const std::string& str, const int val) = 0;
};

};  // namespace Blazar
