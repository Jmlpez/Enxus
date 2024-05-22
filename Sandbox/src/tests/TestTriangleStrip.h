#ifndef TEST_TRIANGLE_STRIP_H
#define TEST_TRIANGLE_STRIP_H

#include "Test.h"

namespace OpenGLTest
{

    class TestTriangleStrip : public Test
    {

    public:
        TestTriangleStrip();
        ~TestTriangleStrip();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        // uint32_t m_VertexBufferID;
        // uint32_t m_IndexBufferID;
        // uint32_t m_VertexArrayID;

        bool m_IsWireFrame = true;

        Enxus::Ref<Enxus::VertexArray> m_VertexArray;

        Enxus::Ref<Enxus::TextureMesh2D> m_Container, m_AwesomeFace;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
