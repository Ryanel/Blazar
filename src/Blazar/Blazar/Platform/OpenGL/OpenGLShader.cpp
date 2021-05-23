#include "bzpch.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Blazar/Platform/OpenGL/OpenGLShader.h"
#include "Blazar/Renderer/Shader.h"

namespace Blazar {

OpenGLShader::OpenGLShader(const std::string& vert_src, const std::string& frag_src) {
    const char* v_str = vert_src.c_str();
    const char* f_str = frag_src.c_str();

    unsigned int vertexShader, fragmentShader;

    int success;
    char infoLog[512];

    // Create temporary shader handles
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile vertex shader
    glShaderSource(vertexShader, 1, &v_str, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG_CORE_ERROR("Vertex Shader Failed to Compile: {}", infoLog);
    }

    // Compile fragment shader
    glShaderSource(fragmentShader, 1, &f_str, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG_CORE_ERROR("Fragment Shader Failed to Compile: {}", infoLog);
    }

    m_Id = glCreateProgram();
    glAttachShader(m_Id, vertexShader);
    glAttachShader(m_Id, fragmentShader);
    glLinkProgram(m_Id);

    
    #ifdef BLAZAR_DEBUG
    glObjectLabel(GL_PROGRAM, m_Id, -1, fmt::format("Blazar Shader {}", m_Id).c_str());
    #endif

    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Ref<OpenGLShader> OpenGLShader::FromText(std::string vertex, std::string fragment) {
    return std::make_shared<OpenGLShader>(vertex, fragment);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_Id); }

void OpenGLShader::Bind() const { glUseProgram(m_Id); }

void OpenGLShader::Unbind() const { glUseProgram(0); }


void OpenGLShader::SetInt(const std::string& name, const int val) {
    GLint location = glGetUniformLocation(m_Id, name.c_str());
    glUniform1i(location, val);
}


void OpenGLShader::SetFloat(const std::string& name, const float val) {
    GLint location = glGetUniformLocation(m_Id, name.c_str());
    glUniform1f(location, val);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& vec) {
    GLint location = glGetUniformLocation(m_Id, name.c_str());
    glUniform2f(location, vec.x, vec.y);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec) {
    GLint location = glGetUniformLocation(m_Id, name.c_str());
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec) {
    GLint location = glGetUniformLocation(m_Id, name.c_str());
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
    GLint location = glGetUniformLocation(m_Id, name.c_str());
    glUniformMatrix3fv(location, 1, false, glm::value_ptr(matrix));
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
    GLint location = glGetUniformLocation(m_Id, name.c_str());
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
}

}  // namespace Blazar
