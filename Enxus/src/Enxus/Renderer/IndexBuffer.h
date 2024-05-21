#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "Base.h"
namespace Enxus
{

    class IndexBuffer
    {

    public:
        IndexBuffer(uint32_t *data, uint32_t count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline uint32_t GetCount() const { return m_Count; }

    private:
        uint32_t m_RendererId;
        uint32_t m_Count;
    };
}

#endif