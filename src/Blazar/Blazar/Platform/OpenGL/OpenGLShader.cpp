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

    CacheUniforms();
}

Ref<OpenGLShader> OpenGLShader::FromText(std::string vertex, std::string fragment) {
    return std::make_shared<OpenGLShader>(vertex, fragment);
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_Id); }

void OpenGLShader::Bind() const { glUseProgram(m_Id); }

void OpenGLShader::Unbind() const { glUseProgram(0); }

void OpenGLShader::SetInt(const std::string& name, const int val) { glUniform1i(GetUniformLocation(name), val); }
void OpenGLShader::SetFloat(const std::string& name, const float val) { glUniform1f(GetUniformLocation(name), val); }

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& vec) {
    glUniform2f(GetUniformLocation(name), vec.x, vec.y);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec) {
    glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec) {
    glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix));
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix));
}

void OpenGLShader::CacheUniforms() {
    // Modified from answer by Nichol Bolas on 
    // https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade

    GLint numActiveUniforms = 0;
    glGetProgramInterfaceiv(m_Id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    std::vector<GLchar> nameData(256);
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);

    std::vector<GLint> values(properties.size());
    for (int location = 0; location < numActiveUniforms; ++location) {
        glGetProgramResourceiv(m_Id, GL_UNIFORM, location, properties.size(), &properties[0], values.size(), NULL,
                               &values[0]);

        nameData.resize(values[0]);  // The length of the name.
        glGetProgramResourceName(m_Id, GL_UNIFORM, location, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0], nameData.size() - 1);

        m_UniformMap[name] = location;
    }
}

int OpenGLShader::GetUniformLocation(const std::string& name) const {
    auto& result = m_UniformMap.find(name);

#ifdef BLAZAR_DEBUG
    if (result == m_UniformMap.end()) { LOG_CORE_WARN("Attempted to get undefined uniform \"{}\" in shader {}.", name, m_Id); }
#endif

    return (result != m_UniformMap.end()) ? result->second : -1;
}

}  // namespace Blazar
