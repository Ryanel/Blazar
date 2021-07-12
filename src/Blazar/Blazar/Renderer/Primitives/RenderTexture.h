#pragma once

#include <vector>
#include "Blazar/Log.h"

#include "Texture.h"

namespace Blazar {

struct RenderTextureProperties {
    int width;
    int height;
    int msaa;

    RenderTextureProperties() : width(1), height(1), msaa(1) {}
    RenderTextureProperties(int width, int height, int msaa = 1) : width(width), height(height), msaa(msaa) {}
};

class RenderTexture {
   public:
    virtual ~RenderTexture() {}
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual uint32_t GetId() const = 0;

    Ref<Texture2D> m_ColorTexture = nullptr;
    static Ref<RenderTexture> Create(const RenderTextureProperties& properties = RenderTextureProperties());
};

};  // namespace Blazar
