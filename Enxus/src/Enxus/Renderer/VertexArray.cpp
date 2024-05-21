#include "pch.h"
#include "VertexArray.h"
#include "utils.h"

namespace Enxus
{

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        default:
            break;
        }
        std::cout << "[ShaderDataType Error] : Unknow ShaderDataType" << std::endl;
        ASSERT(false);
        return 0;
    }

    VertexArray::VertexArray()
    {
        m_VertexAttribIndex = 0;
        GLCall(glGenVertexArrays(1, &m_RendererId));
        GLCall(glBindVertexArray(m_RendererId));
    }
    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &m_RendererId));
    }

    void VertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
    {

        if (vertexBuffer->GetLayout().GetElements().empty())
        {
            std::cout << "[VertexArray Error]: The Vertex Buffer has no layout set" << std::endl;
            ASSERT(false);
        }

        GLCall(glBindVertexArray(m_RendererId));
        vertexBuffer->Bind();

        const BufferLayout &layout = vertexBuffer->GetLayout();

        for (const auto &element : layout)
        {
            switch (element.Type)
            {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            {
                GLCall(glEnableVertexAttribArray(m_VertexAttribIndex));
                GLCall(glVertexAttribPointer(
                    m_VertexAttribIndex,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void *)(intptr_t)element.Offset));

                m_VertexAttribIndex++;
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            {
                // In this case there are no Normalization
                // glVertexAttribIPointer
                // --------------|------ (are not the same function)
                // glVertexAttribPointer
                GLCall(glEnableVertexAttribArray(m_VertexAttribIndex));
                GLCall(glVertexAttribIPointer(
                    m_VertexAttribIndex,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    layout.GetStride(),
                    (const void *)(intptr_t)element.Offset));

                m_VertexAttribIndex++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                break;
            // to avoid switch warning
            case ShaderDataType::None:
            case ShaderDataType::Bool:
                break;
            }
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
    {
        Bind();
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }

    void VertexArray::Bind() const
    {
        GLCall(glBindVertexArray(m_RendererId));
    }
    void VertexArray::Unbind() const
    {
        GLCall(glBindVertexArray(0));
    }
}