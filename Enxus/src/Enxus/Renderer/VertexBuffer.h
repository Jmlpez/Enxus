#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "Base.h"

namespace Enxus
{
    class VertexBuffer
    {

    public:
        VertexBuffer(const void *data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_RendererId;
    };
}
#endif