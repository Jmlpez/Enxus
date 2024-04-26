#ifndef TEST_SKY_BOX_H
#define TEST_SKY_BOX_H

#include "Test.h"

namespace OpenGLTest
{

    class TestSkyBox : public Test
    {
    public:
        TestSkyBox();
        ~TestSkyBox();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        uint32_t LoadCubeMap(std::array<std::string, 6> faces);

    private:
        uint32_t m_SkyBoxRendererId;

        Enxus::Ref<Enxus::VertexArray> m_SkyBoxVAO;
        Enxus::Ref<Enxus::VertexBuffer> m_SkyBoxVBO;

        Enxus::Ref<Enxus::Model> m_Box;
        Enxus::Ref<Enxus::Shader> m_SkyBoxShader, m_BoxShader;
    };

}
#endif