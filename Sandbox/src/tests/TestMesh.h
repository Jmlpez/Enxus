#ifndef TEST_MESH_H
#define TEST_MESH_H

#include "Test.h"

namespace OpenGLTest
{

    class TestMesh : public Test
    {

    public:
        TestMesh();
        ~TestMesh();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        Enxus::Ref<Enxus::Mesh> m_Plane;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
