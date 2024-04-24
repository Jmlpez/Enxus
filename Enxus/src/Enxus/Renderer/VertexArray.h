#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Enxus
{

    class VertexArray
    {

    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer &vbo, const VertexBufferLayout &layout);

        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_RendererId;
    };
}

#endif