#include "pch.h"
#include "TestCircle.h"

namespace OpenGLTest
{
    TestCircle::TestCircle()
    {
        float vertices[] = {
            -1.0f, 1.0f, // top left
            1.0f, 1.0f,  // top right
            1.0f, -1.0f, // bottom right

            1.0f, -1.0f,  // bottom right
            -1.0f, -1.0f, // bottom left
            -1.0f, 1.0f,  // top left
        };

        m_VAO = Enxus::CreateRef<Enxus::VertexArray>();
        m_VBO = Enxus::CreateRef<Enxus::VertexBuffer>(vertices, sizeof(vertices));

        Enxus::VertexBufferLayout layout;
        layout.Push(2, GL_FLOAT);
        m_VAO->AddBuffer(m_VBO, layout);

        m_CircleShader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/circle/circle.vert", "Sandbox/res/shaders/circle/circle.frag");

        glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        m_CircleShader->SetMat4("uProj", ortho);
    }

    TestCircle::~TestCircle()
    {
    }

    void TestCircle::OnUpdate(Enxus::Camera &camera)
    {
        glm::mat4 ortho = glm::ortho(-1.0f * camera.GetAspectRatio(), 1.0f * camera.GetAspectRatio(), -1.0f, 1.0f, -1.0f, 1.0f);
        m_CircleShader->SetMat4("uProj", ortho);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void TestCircle::OnImGuiRender() {

    };

}
