#include "bzpch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Blazar/Platform/OpenGL/OpenGLTexture.h"
#include "Blazar/Renderer/Texture.h"

namespace Blazar {
OpenGLTexture2D::OpenGLTexture2D() {}
OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const TextureProperties& properties)
    : m_Properties(properties) {
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

    int filterMode = m_Properties.filtering == TextureFilterMode::Bilinear ? GL_LINEAR : GL_NEAREST;
    glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, filterMode);
    glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, filterMode);

    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S,
                        properties.wrap_x == TextureWrappingMode::Repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);
    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T,
                        properties.wrap_y == TextureWrappingMode::Repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);

    // Upload
    glTextureSubImage2D(m_Id, 0, 0, 0, m_Width, m_Height, fileFormat, GL_UNSIGNED_BYTE, data);

    LOG_CORE_TRACE("Create Texture {} with width: {}, height: {}, mips: {}, channels: {}", path, width, height, mipmaps,
                   m_Channels);
#ifdef BLAZAR_DEBUG
    glObjectLabel(GL_TEXTURE, m_Id, -1, path.c_str());
#endif

    if (!retainTexture) { stbi_image_free(data); }
}

OpenGLTexture2D* OpenGLTexture2D::FromData(std::vector<char>& fdata, const TextureProperties& properties) {

    OpenGLTexture2D* tex = new OpenGLTexture2D();
    // Hardcoded Parameters, TODO
    bool retainTexture = false;
    int mipmaps = 1;

    // Load texture
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load_from_memory((const stbi_uc*)&fdata.front(), (int)fdata.size(), &width, &height, &channels, 0);

    // TODO: Proper error handling code here
    BLAZAR_CORE_ASSERT(data, "Failed to load image");
    tex->m_Width = width;
    tex->m_Height = height;
    tex->m_Channels = channels;
    tex->m_Properties = properties;

    // Create Textures
    uint32_t gpuFormat = tex->m_Channels == 3 ? GL_RGB8 : GL_RGBA8;
    uint32_t fileFormat = tex->m_Channels == 3 ? GL_RGB : GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &tex->m_Id);
    glTextureStorage2D(tex->m_Id, mipmaps, gpuFormat, tex->m_Width, tex->m_Height);

    // Params

    int filterMode = tex->m_Properties.filtering == TextureFilterMode::Bilinear ? GL_LINEAR : GL_NEAREST;
    glTextureParameteri(tex->m_Id, GL_TEXTURE_MIN_FILTER, filterMode);
    glTextureParameteri(tex->m_Id, GL_TEXTURE_MAG_FILTER, filterMode);

    glTextureParameteri(tex->m_Id, GL_TEXTURE_WRAP_S,
                        properties.wrap_x == TextureWrappingMode::Repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);
    glTextureParameteri(tex->m_Id, GL_TEXTURE_WRAP_T,
                        properties.wrap_y == TextureWrappingMode::Repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER);

    // Upload
    glTextureSubImage2D(tex->m_Id, 0, 0, 0, tex->m_Width, tex->m_Height, fileFormat, GL_UNSIGNED_BYTE, data);

    LOG_CORE_TRACE("[OGL] Texture created (width: {}, height: {}, mips: {}, channels: {})", width, height, mipmaps,
                   tex->m_Channels);

    if (!retainTexture) { stbi_image_free(data); }

    return tex;
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_Id); }
void OpenGLTexture2D::Bind(uint32_t slot) const { glBindTextureUnit(slot, m_Id); }

}  // namespace Blazar
