#ifndef TEST_SHADOW_MAPPING_H
#define TEST_SHADOW_MAPPING_H

#include "Test.h"

namespace OpenGLTest
{
    class ShadowMapFBO
    {
    public:
        ShadowMapFBO(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height)
        {
            GLCall(glGenFramebuffers(1, &m_RendererId));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));

            GLCall(glGenTextures(1, &m_DepthMap));
            GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthMap));
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width,
                                m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

            float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
            GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                          m_DepthMap, 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cout << "Framebuffer is incomplete!" << std::endl;
                ASSERT(false);
            }

            GLCall(glDrawBuffer(GL_NONE));
            GLCall(glReadBuffer(GL_NONE));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
        ~ShadowMapFBO()
        {
            GLCall(glDeleteFramebuffers(1, &m_RendererId));
            GLCall(glDeleteTextures(1, &m_DepthMap));
        };

        void Bind()
        {
            GLCall(glViewport(0, 0, m_Width, m_Height));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId));
            GLCall(glClear(GL_DEPTH_BUFFER_BIT));
        }
        void Unbind()
        {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
        uint32_t GetDepthMapRendererId() { return m_DepthMap; }

    private:
        uint32_t m_RendererId; // framebuffer id
        uint32_t m_DepthMap;   // texture id
        uint32_t m_Width, m_Height;
    };

    class TestShadowMapping : public Test
    {

    public:
        TestShadowMapping();
        ~TestShadowMapping();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnShadowPass() override;
        void OnImGuiRender() override;

    private:
        glm::mat4 m_LightSpaceMatrix, m_LightProjectionMatrix;
        glm::vec3 m_LightDirection, m_LightPosition;
        float m_ShadowBias = 0.0005f;

        Enxus::Scope<ShadowMapFBO> m_ShadowMapFramebuffer;

        Enxus::Ref<Enxus::Model> m_Floor;
        Enxus::Ref<Enxus::Model> m_Box;
        Enxus::Ref<Enxus::Shader> m_Shader, m_ShadowMapDepthShader;
    };
}
#endif
