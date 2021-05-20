#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Blazar {

class Shader {
   public:
    // Factory methods
    static Shader * FromFile(std::string path);
    static Shader * FromText(std::string vertex, std::string fragment);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetMat4(const std::string& name, const glm::mat4& matrix);

   private:
    Shader(const std::string& vert_src, const std::string& frag_src);
    uint32_t m_Id;
};

};  // namespace Blazar
