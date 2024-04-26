#ifndef TEST_MULTIPLE_LIGHT_SOURCES_H
#define TEST_MULTIPLE_LIGHT_SOURCES_H

#include "Test.h"

namespace OpenGLTest
{
    class TestMultipleLightSources : public Test
    {
    public:
        TestMultipleLightSources();
        ~TestMultipleLightSources() override;

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        Enxus::Ref<Enxus::Shader> objShader, lightSourceShader;
        Enxus::Ref<Enxus::VertexArray> lightSourceVAO, objVAO;
        Enxus::Ref<Enxus::Texture2D> containerDiffuse, containerSpecular;
        Enxus::Ref<Enxus::VertexBuffer> objVBO;

        glm::vec3 cubePositions[10] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f),
        };
        glm::vec3 pointLightPositions[4] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f),
        };
    };

}

#endif