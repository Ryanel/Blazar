#pragma once

namespace Blazar {

enum class ShaderDataType : uint8_t {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

uint32_t ShaderDataTypeSize(ShaderDataType type);

};  // namespace Blazar
