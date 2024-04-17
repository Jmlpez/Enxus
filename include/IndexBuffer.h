#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "base.h"
namespace Enxus
{

    class IndexBuffer
    {
    private:
        unsigned int m_RendererId;
        unsigned int m_Count;

    public:
        IndexBuffer(unsigned int *data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const { return m_Count; }
    };
}

#endif