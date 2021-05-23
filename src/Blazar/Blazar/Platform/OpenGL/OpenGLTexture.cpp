#include "bzpch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Blazar/Platform/OpenGL/OpenGLTexture.h"
#include "Blazar/Renderer/Texture.h"

namespace Blazar {

// ----------------------------------------------------
// Vertex Buffer
// ----------------------------------------------------
OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path) {
    // Hardcoded Parameters, TODO
    bool retainTexture = false;
    int mipmaps = 1;

    // Load texture
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    // TODO: Proper error handling code here
    BLAZAR_CORE_ASSERT(data, "Failed to load image");

    m_Width = width;
    m_Height = height;
    m_Channels = channels;

    // Create Textures
    uint32_t gpuFormat = m_Channels == 3 ? GL_RGB8 : GL_RGBA8;
    uint32_t fileFormat = m_Channels == 3 ? GL_RGB : GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
    glTextureStorage2D(m_Id, mipmaps, gpuFormat, m_Width, m_Height);

    // Params
    glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Upload
    glTextureSubImage2D(m_Id, 0, 0, 0, m_Width, m_Height, fileFormat, GL_UNSIGNED_BYTE, data);

    #ifdef BLAZAR_DEBUG
    glObjectLabel(GL_TEXTURE, m_Id, -1, path.c_str());
    #endif

    if (!retainTexture) { stbi_image_free(data); }
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_Id); }
void OpenGLTexture2D::Bind(uint32_t slot) const { glBindTextureUnit(slot, m_Id); }

}  // namespace Blazar
