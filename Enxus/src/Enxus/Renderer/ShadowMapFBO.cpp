#include "pch.h"
#include "ShadowMapFBO.h"
#include "utils.h"

namespace Enxus
{
    ShadowMapFBO::ShadowMapFBO(ShadowMapSpecification spec)
        : m_Specification(spec)
    {
        Invalidate();
    }

    ShadowMapFBO::~ShadowMapFBO()
    {
        GLCall(glDeleteFramebuffers(1, &m_RendererId));
        GLCall(glDeleteTextures(1, &m_DepthMapId));
    };

    void ShadowMapFBO::Invalidate()
    {
        GLCall(glGenFramebuffers(1, &m_RendererId));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));

        GLCall(glGenTextures(1, &m_DepthMapId));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthMapId));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Specification.Width,
                            m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                      m_DepthMapId, 0));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Framebuffer is incomplete!" << std::endl;
            ASSERT(false);
        }

        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void ShadowMapFBO::Bind() const
    {
        GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    }
    void ShadowMapFBO::BindShadowTexture(uint32_t slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthMapId));
    }
    void ShadowMapFBO::Unbind() const
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

}
