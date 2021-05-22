#pragma once

#include <glm/glm.hpp>

#include <string>

#include "Blazar/Renderer/Shader.h"

namespace Blazar {

class OpenGLShader : public Shader {
   public:
    static Ref<OpenGLShader> FromText(std::string vertex, std::string fragment);

    virtual ~OpenGLShader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetFloat(const std::string& name, const float val);
    void SetFloat2(const std::string& name, const glm::vec2& vec);
    void SetFloat3(const std::string& name, const glm::vec3& vec);
    void SetFloat4(const std::string& name, const glm::vec4& vec);

    void SetMat3(const std::string& name, const glm::mat3& matrix);
    void SetMat4(const std::string& name, const glm::mat4& matrix);

    OpenGLShader(const std::string& vert_src, const std::string& frag_src);
   private:
    uint32_t m_Id;
};

};  // namespace Blazar
