#ifndef TEST_MULTIPLE_LIGHT_SOURCES_H
#define TEST_MULTIPLE_LIGHT_SOURCES_H

#include "Test.h"
#include "utils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"

namespace Test
{
    class TestMultipleLightSources : public Test
    {
    public:
        TestMultipleLightSources();
        ~TestMultipleLightSources() override;

        void OnUpdate(float deltaTime, FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        Shader *objShader, *lightSourceShader;
        VertexArray *lightSourceVAO, *objVAO;
        Texture2D *containerDiffuse, *containerSpecular;
        VertexBuffer *objVBO;

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