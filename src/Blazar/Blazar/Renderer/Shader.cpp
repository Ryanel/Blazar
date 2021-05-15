#include "bzpch.h"

#include "Blazar/Renderer/Shader.h"

#include <glad/glad.h>

namespace Blazar {

Shader::Shader(const std::string& vert_src, const std::string& frag_src) {
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

    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


Shader Shader::FromFile(std::string path) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(path + ".vs");
        fShaderFile.open(path + ".fs");

        // read file's buffer contents into streams
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) { std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl; }

    return Shader(vertexCode, fragmentCode);
}

Shader Shader::FromText(std::string vertex, std::string fragment) { return Shader(vertex, fragment); }

Shader::~Shader() { glDeleteProgram(m_Id); }

void Shader::Bind() const { glUseProgram(m_Id); }

void Shader::Unbind() const { glUseProgram(0); }

}  // namespace Blazar