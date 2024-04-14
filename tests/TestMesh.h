#ifndef TEST_MESH_H
#define TEST_MESH_H

#include "Test.h"
#include "Shader.h"
// #include "Mesh.h"
#include "Renderer.h"

namespace Test
{

    class TestMesh : public Test
    {

    public:
        TestMesh();
        ~TestMesh();

        void OnUpdate(float deltaTime, FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        Ref<Mesh> m_Plane;
        Ref<Shader> m_Shader;
    };
}
#endif
