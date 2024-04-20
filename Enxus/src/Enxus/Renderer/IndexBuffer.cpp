#include "pch.h"
#include "IndexBuffer.h"
#include "utils.h"

namespace Enxus
{

    IndexBuffer::IndexBuffer(unsigned int *data, unsigned int count) : m_Count(count)
    {
        GLCall(glGenBuffers(1, &m_RendererId));

        Bind();
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_RendererId));
    }

    void IndexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    }
    void IndexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}