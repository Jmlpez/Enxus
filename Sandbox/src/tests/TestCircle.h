#ifndef TEST_CIRCLE_H
#define TEST_CIRCLE_H

#include "Test.h"

namespace OpenGLTest
{

    class TestCircle : public Test
    {

    public:
        TestCircle();
        ~TestCircle();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        Enxus::Ref<Enxus::VertexArray> m_VAO;
        Enxus::Ref<Enxus::VertexBuffer> m_VBO;
        Enxus::Ref<Enxus::Shader> m_CircleShader;
    };
}
#endif
