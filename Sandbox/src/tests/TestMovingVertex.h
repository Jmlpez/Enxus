#ifndef TEST_MOVING_VERTEX_H
#define TEST_MOVING_VERTEX_H

#include "Test.h"

namespace OpenGLTest
{

    class TestMovingVertex : public Test
    {

    public:
        TestMovingVertex();
        ~TestMovingVertex();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        uint32_t m_VertexBufferID;
        uint32_t m_IndexBufferID;
        uint32_t m_VertexArrayID;

        std::vector<Enxus::VertexData> m_Vertices;

        Enxus::Ref<Enxus::Texture2D> m_Container, m_AwesomeFace;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
