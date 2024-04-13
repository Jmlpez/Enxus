#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include "Test.h"
#include "Shader.h"
#include "Model.h"
#include "Renderer.h"

namespace Test
{

    class TestModel : public Test
    {

    public:
        TestModel();
        ~TestModel();

        // Not used
        void OnUpdate(float deltaTime, FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::shared_ptr<Model> m_Backpack;
        std::shared_ptr<Model> m_Box;
        std::shared_ptr<Shader> m_Shader;

        glm::vec3 m_LightDirection;
        bool m_IsUVsFlipped = true;
    };
}
#endif
