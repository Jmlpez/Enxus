#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include "Test.h"

namespace OpenGLTest
{

    class TestModel : public Test
    {

    public:
        TestModel();
        ~TestModel();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        std::vector<Enxus::Ref<Enxus::Model>> m_ModelsList;

        Enxus::Ref<Enxus::Shader> m_Shader;

        glm::vec3 m_LightDirection;
        bool m_IsUVsFlipped = false;
    };
}
#endif
