#pragma once

#include <vector>

#include "ShaderDataType.h"

namespace Blazar {

/// Describes an element of a Vertex Buffer Layout (e.g: A normalized Mat4 12 bytes offset from the stride).
struct BufferElement {
    std::string Name;
    uint32_t Size;
    uint32_t Offset;
    ShaderDataType Type;
    bool Normalized;

    BufferElement() : Name(""), Size(0), Offset(0), Type(ShaderDataType::None), Normalized(false) {}

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

    uint32_t GetElementCount() const {
        switch (Type) {
            case ShaderDataType::Int:
            case ShaderDataType::Float:
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::Int2:
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Int3:
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Int4:
            case ShaderDataType::Float4:
                return 4;
            case ShaderDataType::Mat3:
                return 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4;
            default:
                BLAZAR_CORE_ASSERT(false, "Unknown ShaderDataType!");
                return 0;
        }
    }
};

/// Describes the layout of a Vertex Buffer
class BufferLayout {
   public:
    BufferLayout() {}
    BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) { CalculateOffsetsAndStride(); }

    inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
    uint32_t GetStride() const { return m_Stride; }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

   private:
    void CalculateOffsetsAndStride() {
        uint32_t offset = 0;
        m_Stride = 0;

        for (auto& element : m_Elements) {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

   private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class VertexBuffer {
   public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetLayout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& GetLayout() const = 0;

    // Factory method
    static Ref<VertexBuffer> Create(float* verticies, size_t size);
    static Ref<VertexBuffer> Create(float* verticies, size_t size, BufferLayout& layout);
};

class IndexBuffer {
   public:
    virtual ~IndexBuffer() {}
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual size_t GetCount() const = 0;
    // Factory method
    static Ref<IndexBuffer> Create(uint32_t* verticies, size_t size);
};

};  // namespace Blazar
