#ifndef TEST_POISSON_SAMPLING_DISK_H
#define TEST_POISSON_SAMPLING_DISK_H

#include "Test.h"

namespace OpenGLTest
{

    class TestPoissonSamplingDisk : public Test
    {

    public:
        TestPoissonSamplingDisk();
        ~TestPoissonSamplingDisk();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        void CalculateGrid();

    private:
        std::array<glm::mat4, 10000> m_BoxInstanceMatrix;

        Enxus::Ref<Enxus::Model> m_Box;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
