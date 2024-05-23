#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "BufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Enxus
{

    class VertexArray
    {

    public:
        VertexArray();
        ~VertexArray();

        void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer);
        void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer);

        const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const { return m_VertexBuffers; }
        const Ref<IndexBuffer> &GetIndexBuffer() const { return m_IndexBuffer; }

        inline void ResetVertexAttribIndex(uint8_t index = 0) { m_VertexAttribIndex = index; }

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_RendererId;
        uint32_t m_VertexAttribIndex;

        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}

#endif