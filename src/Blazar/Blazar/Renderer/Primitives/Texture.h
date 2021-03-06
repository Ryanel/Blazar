#pragma once

#include <vector>
#include "imgui.h"

#include "Blazar/Assets/Resource.h"
#include "Blazar/Memory.h"
namespace Blazar {

/// Determines whether a texture's U or V axis will repeat or clamp
enum class TextureWrappingMode { Clamp, Repeat };

/// The texture filtering mode, how smooth or pixilated the texture is
enum class TextureFilterMode { None, Bilinear };

enum class TextureSourceType { RAW_32BIT_RGBA, PNG };

struct TextureProperties {
    TextureWrappingMode wrap_x      = TextureWrappingMode::Clamp;
    TextureWrappingMode wrap_y      = TextureWrappingMode::Clamp;
    TextureFilterMode   filtering   = TextureFilterMode::Bilinear;
    TextureSourceType   source_type = TextureSourceType::PNG;
};

class Texture {
   public:
    virtual ~Texture() {}
    virtual uint32_t GetWidth() const  = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void        Bind(uint32_t slot = 0) const = 0;
    virtual uint32_t    GetId() const                 = 0;
    virtual ImTextureID imgui_id() const              = 0;

    std::string path;
};

class Texture2D : public Texture {
   public:
    // Factory method
    static Ref<Texture> Create(const std::string& path, const TextureProperties& properties = TextureProperties());
    static Ref<Resource<Texture2D>> Load(const std::string& path, TextureProperties props = TextureProperties());
};

};  // namespace Blazar
