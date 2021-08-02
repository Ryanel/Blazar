#include "bzpch.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Blazar/Assets/ResourceManager.h"
#include "Blazar/Renderer/Primitives/Shader.h"
#include "Blazar/Renderer/Renderer.h"

#include "Blazar/Platform/OpenGL/OpenGLShader.h"

namespace Blazar {

Ref<Shader> Shader::Load(std::string path) {
    std::string   vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        auto& vfs = ResourceManager::get()->m_vfs;

        std::vector<std::byte> vsOutBuffer;
        std::vector<std::byte> fsOutBuffer;

        vsOutBuffer.reserve(512);
        fsOutBuffer.reserve(512);

        if (!ResourceManager::get()->vfs_read(path + ".vs", vsOutBuffer)) {
            LOG_CORE_ERROR("Shader: Failed to read fragment shader");
        }

        if (!ResourceManager::get()->vfs_read(path + ".fs", fsOutBuffer)) {
            LOG_CORE_ERROR("Shader: Failed to read vertex shader");
        }

        std::stringstream vertex;
        for (size_t i = 0; i < vsOutBuffer.size(); i++)
        {
            vertex << (char)vsOutBuffer[i];
        }
        vertexCode = vertex.str();

        std::stringstream frag;
        for (size_t i = 0; i < fsOutBuffer.size(); i++)
        {
            frag << (char)fsOutBuffer[i];
        }
        fragmentCode = frag.str();

        // convert stream into string
        //vertexCode   = vShaderStream.str();
        //fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) { LOG_CORE_ERROR("Shader: Unable to read file. Reason: {}", e.what()); }

    return Shader::FromText(vertexCode, fragmentCode);
}

Ref<Shader> Shader::FromText(std::string vertex, std::string fragment) {
    switch (Renderer::api()) {
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
