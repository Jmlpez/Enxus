#ifndef TEST_DEPTH_BUFFER_H
#define TEST_DEPTH_BUFFER_H

#include "Test.h"
#include "Shader.h"
#include "Renderer.h"

namespace Test
{

    class TestDepthBuffer : public Test
    {

    public:
        TestDepthBuffer();
        ~TestDepthBuffer();

        // Not used
        void OnUpdate(float deltaTime, FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::shared_ptr<Mesh> m_Box, m_Floor;
        std::shared_ptr<Shader> m_NormalShader, m_SingleColorShader;

        bool showDepthValue = false;
        float m_OutlineWidth = 0.1f;
    };
}
#endif
