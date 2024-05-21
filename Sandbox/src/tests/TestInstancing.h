#ifndef TEST_INSTANCING_H
#define TEST_INSTANCING_H

#include "Test.h"

namespace OpenGLTest
{

    class TestInstancing : public Test
    {

    public:
        TestInstancing();
        ~TestInstancing();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        glm::vec2 m_QuadTranslations[100];

        Enxus::Ref<Enxus::VertexArray> m_VAO;
        Enxus::Ref<Enxus::VertexBuffer> m_VBO, m_InstancedBuffer;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
