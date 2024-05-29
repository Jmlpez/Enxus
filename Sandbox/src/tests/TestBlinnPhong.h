#ifndef TEST_BLINN_PHONG_H
#define TEST_BLINN_PHONG_H

#include "Test.h"

namespace OpenGLTest
{

    class TestBlinnPhong : public Test
    {

    public:
        TestBlinnPhong();
        ~TestBlinnPhong();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        Enxus::Ref<Enxus::Mesh> m_Floor;
        Enxus::Ref<Enxus::Shader> m_Shader;

        glm::vec3 m_LightPosition;

        bool m_UseBlinnPhong;
    };
}
#endif
