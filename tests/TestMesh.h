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

        // Not used
        void OnUpdate(float deltaTime, FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::shared_ptr<Mesh> m_Plane;
        std::shared_ptr<Shader> m_Shader;
    };
}
#endif
