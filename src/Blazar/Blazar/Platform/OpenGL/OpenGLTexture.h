#pragma once

#include <cstdint>

#include "Blazar/Renderer/Texture.h"

namespace Blazar {

class OpenGLTexture2D : public Texture2D {
   public:
    OpenGLTexture2D();
    OpenGLTexture2D(const std::string& path, const TextureProperties& properties);

    static OpenGLTexture2D* FromData(std::vector<char>& data, const TextureProperties& properties);
    virtual ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; };

    virtual void Bind(uint32_t slot = 0) const override;

    virtual uint32_t GetId() const { return m_Id; }
    // virtual void Unbind() const override;

   private:
    uint32_t m_Id;
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Channels;

    TextureProperties m_Properties;
};

};  // namespace Blazar
