#include "pch.h"
#include "VertexBuffer.h"
#include "utils.h"

namespace Enxus
{

    VertexBuffer::VertexBuffer(uint32_t size)
    {
        GLCall(glGenBuffers(1, &m_RendererId));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    VertexBuffer::VertexBuffer(const void *data, uint32_t size)
    {
        GLCall(glGenBuffers(1, &m_RendererId));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }

    VertexBuffer::~VertexBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_RendererId));
    }

    void VertexBuffer::SetData(const void *data, uint32_t size, uint32_t offset)
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data))
    }

    void VertexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
    }
    void VertexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}