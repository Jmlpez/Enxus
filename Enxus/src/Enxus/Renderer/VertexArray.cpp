#include "pch.h"
#include "VertexArray.h"
#include "utils.h"

namespace Enxus
{

    VertexArray::VertexArray()
    {
        GLCall(glGenVertexArrays(1, &m_RendererId));
        GLCall(glBindVertexArray(m_RendererId));
    }
    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &m_RendererId));
    }

    void VertexArray::AddBuffer(const Ref<VertexBuffer> &vbo, const VertexBufferLayout &layout)
    {
        Bind();
        vbo->Bind();
        const auto &elements = layout.GetElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++)
        {
            const auto &element = elements[i];
            GLCall(glEnableVertexAttribArray(i));
            auto offsetVoid = (const void *)offset;
            GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), offsetVoid));
            offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        }
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