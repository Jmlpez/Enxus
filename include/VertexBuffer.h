#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "utils.h"

namespace Enxus
{
    class VertexBuffer
    {
    private:
        unsigned int m_RendererId;

    public:
        VertexBuffer(const void *data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
    };
}
#endif