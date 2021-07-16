#include "bzpch.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Blazar/Platform/OpenGL/OpenGLRenderTexture.h"
#include "Blazar/Platform/OpenGL/OpenGLTexture.h"
#include "Blazar/Renderer/Primitives/Texture.h"

namespace Blazar {
OpenGLRenderTexture::OpenGLRenderTexture() {}
OpenGLRenderTexture::OpenGLRenderTexture(const RenderTextureProperties& properties) : m_Properties(properties) {
    ZoneScoped;
    glGenFramebuffers(1, &m_Id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    // Bind color buffer
    {
        m_ColorTexture.reset(
            OpenGLTexture2D::Uninitialized(m_Properties.width, m_Properties.height, Blazar::TextureProperties()));

        // Attach to color
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->GetId(), 0);
    }

    // Bind Depth + Stencil
    {
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Properties.width, m_Properties.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        m_DepthStencilID = rbo;
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    bool isComplete = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    //LOG_CORE_TRACE("Create RenderTexture {} with width: {}, height: {}, msaa: {}", m_Id, m_Properties.width,
    //               m_Properties.height, m_Properties.msaa);

    BLAZAR_CORE_ASSERT(isComplete, "Framebuffer not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGLRenderTexture::~OpenGLRenderTexture() {
    ZoneScoped;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // back to default
    glDeleteFramebuffers(1, &m_Id);

    if (m_DepthStencilID != 0) { glDeleteRenderbuffers(1, &m_DepthStencilID); }

    //LOG_CORE_TRACE("Delete RenderTexture {} with width: {}, height: {}, msaa: {}", m_Id, m_Properties.width,
    //               m_Properties.height, m_Properties.msaa);
}
void OpenGLRenderTexture::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_Id); }

void OpenGLRenderTexture::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }


}  // namespace Blazar
