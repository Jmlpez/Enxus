#ifndef SHADOW_MAP_FBO_H
#define SHADOW_MAP_FBO_H

#include "Base.h"

namespace Enxus
{

    struct ShadowMapSpecification
    {
        uint32_t Width, Height;
        ShadowMapSpecification() : Width(1), Height(1) {}
        ShadowMapSpecification(const ShadowMapSpecification &spec) : Width(spec.Width), Height(spec.Height) {}
        ShadowMapSpecification(uint32_t width, uint32_t height) : Width(width), Height(height) {}
    };

    class ShadowMapFBO
    {
    public:
        ShadowMapFBO(ShadowMapSpecification spec);
        ~ShadowMapFBO();

        void BindShadowTexture(uint32_t slot = 0) const;
        void Bind() const;
        void Unbind() const;
        inline uint32_t GetDepthMapRendererId() { return m_DepthMapId; }

    private:
        void Invalidate();

    private:
        uint32_t m_RendererId; // framebuffer id
        uint32_t m_DepthMapId; // texture id
        ShadowMapSpecification m_Specification;
    };
}
#endif