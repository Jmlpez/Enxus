#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

namespace Enxus
{

    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        // unsigned Samples = 1;
        FramebufferSpecification()
            : Width(1), Height(1) {}
        FramebufferSpecification(uint32_t width, uint32_t height)
            : Width(width), Height(height) {}
    };

    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferSpecification &spec);
        ~Framebuffer();

        // means that state is not valid so recreate it
        void Invalidate();
        void Resize(uint32_t width, uint32_t height);

        void Bind();
        void Unbind();

        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

        FramebufferSpecification GetSpecification() const { return m_Specification; }

    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment, m_DepthAttachment;
        FramebufferSpecification m_Specification;
    };
    /* data */

}
#endif