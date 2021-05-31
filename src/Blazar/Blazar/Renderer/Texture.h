#pragma once

#include <vector>
#include "Blazar/Log.h"
#include "Blazar/Renderer/ShaderDataType.h"

namespace Blazar {

/// Determines whether a texture's U or V axis will repeat or clamp
enum class TextureWrappingMode { Clamp, Repeat };

/// The texture filtering mode, how smooth or pixilated the texture is
enum class TextureFilterMode { None, Bilinear };

struct TextureProperties {
    TextureWrappingMode wrap_x = TextureWrappingMode::Clamp;
    TextureWrappingMode wrap_y = TextureWrappingMode::Clamp;
    TextureFilterMode filtering = TextureFilterMode::None;
};

class Texture {
   public:
    virtual ~Texture() {}
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture {
   public:
    // Factory method
    static Ref<Texture> Create(const std::string& path, const TextureProperties& properties = TextureProperties());
};

};  // namespace Blazar
