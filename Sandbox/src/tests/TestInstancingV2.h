#ifndef TEST_INSTANCING_V2_H
#define TEST_INSTANCING_V2_H

#include "Test.h"

namespace OpenGLTest
{

    class TestInstancingV2 : public Test
    {

    public:
        TestInstancingV2();
        ~TestInstancingV2();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        std::array<glm::mat4, 1000> m_BoxInstanceMatrix;

        //

        Enxus::Ref<Enxus::Model> m_Box;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
