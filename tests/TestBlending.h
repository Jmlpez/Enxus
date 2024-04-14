#ifndef TEST_BLENDING_H
#define TEST_BLENDING_H

#include "utils.h"
#include "Test.h"
#include "Shader.h"
#include "Mesh.h"
#include <array>
#include <map>

namespace Test
{
    class TestBlending : public Test
    {

    public:
        TestBlending();
        ~TestBlending();

        void OnUpdate(float deltaTime, Enxus::FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // private data
        Enxus::Ref<Enxus::Mesh> m_Box, m_Floor, m_TransparentObj;
        Enxus::Ref<Enxus::Shader> m_Shader;

        std::array<glm::vec3, 5> m_TransparentObjsPositions;

        std::map<float, glm::vec3> m_SortedPositions;
    };
}
#endif
