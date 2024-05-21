#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include "Base.h"
#include "utils.h"

namespace Enxus
{
    enum class ShaderDataType
    {
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
        Bool,
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 8;
        case ShaderDataType::Float3:
            return 12;
        case ShaderDataType::Float4:
            return 16;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 8;
        case ShaderDataType::Int3:
            return 12;
        case ShaderDataType::Int4:
            return 16;
        case ShaderDataType::Bool:
            return 1;
            // to avoid switch warning
        case ShaderDataType::None:
            break;
        }
        std::cout << "[ShaderDataType Error] : Unknow ShaderDataType" << std::endl;
        ASSERT(false);
        return 0;
    }

    struct BufferElement
    {
        ShaderDataType Type;
        std::string Name;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;
        BufferElement() {}
        BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
            : Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float:
                return 1;
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Float4:
                return 4;
            case ShaderDataType::Mat3:
                return 3; // 3* float3
            case ShaderDataType::Mat4:
                return 4; // 4* float4
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Bool:
                return 1;
            // to avoid switch warning
            case ShaderDataType::None:
                break;
            }
            std::cout << "[ShaderDataType Error] : Unknow ShaderDataType" << std::endl;
            ASSERT(false);
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout(){};
        BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements), m_Stride(0)
        {
            CalculateOffsetAndStride();
        }

        inline const std::vector<BufferElement> &GetElements() const { return m_Elements; }
        inline uint32_t GetStride() const { return m_Stride; }

        inline std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        inline std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

        inline std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        inline std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void CalculateOffsetAndStride()
        {
            uint32_t offset = 0;
            for (auto &element : m_Elements)
            {
                element.Offset = offset;
                m_Stride += element.Size;
                offset += element.Size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;
    };
}
#endif
