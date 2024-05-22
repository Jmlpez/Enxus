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
        std::array<glm::vec2, 100> m_QuadTranslations;

        Enxus::Ref<Enxus::VertexArray> m_VAO;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
