#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include <string>

#include "Blazar/Renderer/Primitives/Shader.h"

namespace Blazar {

class OpenGLShader : public Shader {
   public:
    // Lifecycle
    OpenGLShader(const std::string& vert_src, const std::string& frag_src);
    virtual ~OpenGLShader();
    static Ref<OpenGLShader> FromText(std::string vertex, std::string fragment);

    // Binding
    void Bind() const;
    void Unbind() const;

    // Uniform setting
    void SetInt(const std::string& name, const int val);

    void SetFloat(const std::string& name, const float val);
    void SetFloat2(const std::string& name, const glm::vec2& vec);
    void SetFloat3(const std::string& name, const glm::vec3& vec);
    void SetFloat4(const std::string& name, const glm::vec4& vec);

    void SetMat3(const std::string& name, const glm::mat3& matrix);
    void SetMat4(const std::string& name, const glm::mat4& matrix);

    std::unordered_map<std::string, int> m_UniformMap;

   private:
    void CacheUniforms();
    int GetUniformLocation(const std::string& name) const;

   private:
    uint32_t m_Id;
};

};  // namespace Blazar
