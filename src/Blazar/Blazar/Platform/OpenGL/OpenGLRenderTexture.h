#pragma once

#include <cstdint>

#include "Blazar/Renderer/RenderTexture.h"

namespace Blazar {

class OpenGLRenderTexture : public RenderTexture {
   public:
    OpenGLRenderTexture();
    OpenGLRenderTexture(const RenderTextureProperties& properties);

    virtual ~OpenGLRenderTexture();

    virtual uint32_t GetWidth() const override { return m_Properties.width; }
    virtual uint32_t GetHeight() const override { return m_Properties.height; };

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual uint32_t GetId() const { return m_Id; }
    virtual uint32_t GetColorId() const { return m_ColorBufferID; }
    // virtual void Unbind() const override;

   private:
    uint32_t m_Id;

    RenderTextureProperties m_Properties;

    uint32_t m_ColorBufferID = 0;
    uint32_t m_DepthStencilID = 0;
};

};  // namespace Blazar
