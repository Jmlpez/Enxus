#include "TestTemplate.h"
#include "imgui/imgui.h"
#include "Renderer.h"

namespace Test
{
    TestTemplate::TestTemplate()
    {
    }

    TestTemplate::~TestTemplate()
    {
    }

    void TestTemplate::OnUpdate(float deltaTime, Enxus::FreeCameraController *cameraController)
    {
        (void)deltaTime;
        // m_Shader->Bind();
        // cameraController->GetCamera()->SetViewProjMatrix(*m_Shader);
    }
    void TestTemplate::OnRender()
    {
        Enxus::Renderer renderer;
        renderer.ClearColor(0.13f, 0.14f, 0.16f, 1.0f);
    };

    void TestTemplate::OnImGuiRender()
    {
    }

}
