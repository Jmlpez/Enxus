#ifndef TEST_BLENDING_H
#define TEST_BLENDING_H

#include "Test.h"
#include "Enxus.h"

namespace OpenGLTest
{
    class TestBlending : public Test
    {

    public:
        TestBlending();
        ~TestBlending();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        // private data
        Enxus::Ref<Enxus::Mesh> m_Box, m_Floor, m_TransparentObj;
        Enxus::Ref<Enxus::Shader> m_Shader;

        std::array<glm::vec3, 5> m_TransparentObjsPositions;
    };
}
#endif
