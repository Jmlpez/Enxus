#ifndef TEST_MESH_H
#define TEST_MESH_H

#include "Test.h"
#include "Shader.h"
#include "Renderer.h"

namespace Test
{

    class TestMesh : public Test
    {

    public:
        TestMesh();
        ~TestMesh();

        void OnUpdate(float deltaTime, Enxus::FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        Enxus::Ref<Enxus::Mesh> m_Plane;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
