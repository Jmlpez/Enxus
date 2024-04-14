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
        void OnUpdate(float deltaTime, Enxus::FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        Enxus::Ref<Enxus::Model> m_Backpack;
        Enxus::Ref<Enxus::Model> m_Box;
        Enxus::Ref<Enxus::Shader> m_Shader;

        glm::vec3 m_LightDirection;
        bool m_IsUVsFlipped = true;
    };
}
#endif
