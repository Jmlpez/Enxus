#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "Base.h"

namespace Enxus
{

    class Texture2D
    {

    public:
        Texture2D(uint32_t width, uint32_t height);
        ~Texture2D();

        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }

        void SetData(void *data, uint32_t width, uint32_t height);
        void Resize(uint32_t width, uint32_t height);

        void Bind(uint32_t slot = 0) const;
        void Unbind() const;

        uint32_t GetRendererId() const { return m_RendererId; }

    private:
        void CreateTexture();

    private:
        uint32_t m_RendererId;
        uint32_t m_Width, m_Height;
    };
}

#endif