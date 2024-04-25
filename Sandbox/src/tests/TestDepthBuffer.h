#ifndef TEST_DEPTH_BUFFER_H
#define TEST_DEPTH_BUFFER_H

#include "Test.h"


namespace OpenGLTest
{

    class TestDepthBuffer : public Test
    {

    public:
        TestDepthBuffer();
        ~TestDepthBuffer();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        Enxus::Ref<Enxus::Mesh> m_Box, m_Floor;
        Enxus::Ref<Enxus::Shader> m_NormalShader, m_SingleColorShader;

        bool showDepthValue = false;
        float m_OutlineWidth = 0.1f;
    };
}
#endif
