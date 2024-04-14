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
        Ref<Model> m_Backpack;
        Ref<Model> m_Box;
        Ref<Shader> m_Shader;

        glm::vec3 m_LightDirection;
        bool m_IsUVsFlipped = true;
    };
}
#endif
