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
        // uint32_t m_VertexBufferID;
        // uint32_t m_IndexBufferID;
        // uint32_t m_VertexArrayID;

        Enxus::Ref<Enxus::VertexBuffer> m_VertexBuffer;
        Enxus::Ref<Enxus::IndexBuffer> m_IndexBuffer;
        Enxus::Ref<Enxus::VertexArray> m_VertexArray;

        std::vector<Enxus::VertexData> m_Vertices;

        float m_DistanceFactor = 3.0f;
        float m_WaveSpeed = 2.0f;
        bool m_IsAnimatingVertices = true;

        Enxus::Ref<Enxus::Texture2D> m_Container, m_AwesomeFace;
        Enxus::Ref<Enxus::Shader> m_Shader;
    };
}
#endif
