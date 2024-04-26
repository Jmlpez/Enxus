#include "pch.h"
#include "Framebuffer.h"
#include "utils.h"

namespace Enxus
{

    Framebuffer::Framebuffer(const FramebufferSpecification &spec)
        : m_RendererID(0), m_ColorAttachment(0), m_DepthAttachment(0), m_Specification(spec)
    {
        Invalidate();
    }

    Framebuffer::~Framebuffer()
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererID));
        GLCall(glDeleteTextures(1, &m_ColorAttachment));
        GLCall(glDeleteTextures(1, &m_DepthAttachment));
    }
    void Framebuffer::Invalidate()
    {
        // if already exist the buffer then remove everything to recreate it
        if (m_RendererID)
        {
            GLCall(glDeleteFramebuffers(1, &m_RendererID));
            GLCall(glDeleteTextures(1, &m_ColorAttachment));
            GLCall(glDeleteTextures(1, &m_DepthAttachment));
        }

        // GLCall(glCreateFramebuffers(1, &m_RendererID));
        GLCall(glGenFramebuffers(1, &m_RendererID));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

        GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

        // Using textures for Depth/Stencil attachment
        GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
        // GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0,
                            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));

        // Using Render Buffer Objects for Depth/Stencil attachment (problems with ImGUI, I'm still figuring out the origin of the problem)
        // GLCall(glGenRenderbuffers(1, &m_DepthAttachment));
        // GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment));
        // GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));
        // GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0)); // unbind
        // GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Framebuffer is incomplete!" << std::endl;
            ASSERT(false);
        }

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::Bind()
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    }

    void Framebuffer::Unbind()
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::Resize(unsigned int width, unsigned int height)
    {
        // std::cout << width << "  " << height << std::endl;
        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }
}
