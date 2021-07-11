#pragma once

#include <vector>
#include "Blazar/Log.h"
#include "Blazar/Renderer/ShaderDataType.h"

namespace Blazar {

struct RenderTextureProperties {
    int width;
    int height;
    int msaa;
};

class RenderTexture {
   public:
    virtual ~RenderTexture() {}
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual uint32_t GetId() const = 0;
    virtual uint32_t GetColorId() const = 0;
    static Ref<RenderTexture> Create(const RenderTextureProperties& properties = RenderTextureProperties());
};

};  // namespace Blazar
