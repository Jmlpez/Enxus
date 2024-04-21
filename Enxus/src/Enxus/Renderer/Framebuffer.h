#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace Enxus
{

    struct FramebufferSpecification
    {
        unsigned int Width, Height;
        unsigned Samples = 1;
    };

    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferSpecification &spec);
        ~Framebuffer();

        // means that state is not valid so recreate it
        void Invalidate();

        void Bind();
        void Unbind();

        unsigned int GetColorAttachmentRendererID() const { return m_ColorAttachment; }

        FramebufferSpecification GetSpecification() const { return m_Specification; }

    private:
        unsigned int m_RendererID;
        unsigned int m_ColorAttachment, m_DepthAttachment;
        FramebufferSpecification m_Specification;
    };
    /* data */

}
#endif