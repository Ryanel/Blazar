#include "bzpch.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Blazar/Renderer/Shader.h"
#include "Blazar/Renderer/Renderer.h"

#include "Blazar/Platform/OpenGL/OpenGLShader.h"

namespace Blazar {

Shader* Shader::FromFile(std::string path) {
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
    } catch (std::ifstream::failure e) { LOG_CORE_ERROR("Shader: Unable to read file. Reason: {}", e.what()); }

    return Shader::FromText(vertexCode, fragmentCode);
}

Shader* Shader::FromText(std::string vertex, std::string fragment) {
    switch(Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return OpenGLShader::FromText(vertex, fragment);
        case RendererAPI::API::None:
            BLAZAR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        default:
            BLAZAR_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
    }

}

}  // namespace Blazar
