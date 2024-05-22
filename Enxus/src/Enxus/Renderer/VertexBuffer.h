#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "Base.h"
#include "BufferLayout.h"

namespace Enxus
{
    class VertexBuffer
    {

    public:
        VertexBuffer(uint32_t size);
        VertexBuffer(const void *data, uint32_t size);
        ~VertexBuffer();

        void SetLayout(BufferLayout layout) { m_Layout = layout; }
        const BufferLayout &GetLayout() const { return m_Layout; }
        void SetData(const void *data, uint32_t size, uint32_t offset = 0);

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_RendererId;
        BufferLayout m_Layout;
    };
}
#endif