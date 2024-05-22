#include "pch.h"
#include "TestInstancing.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestInstancing::TestInstancing()
    {
        float quadVertices[] = {

            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.0f, 1.0f, 1.0f};

        m_VAO = Enxus::CreateRef<Enxus::VertexArray>();
        Enxus::Ref<Enxus::VertexBuffer> quadVBO = Enxus::CreateRef<Enxus::VertexBuffer>(quadVertices, sizeof(quadVertices));

        Enxus::BufferLayout layout = {
            {Enxus::ShaderDataType::Float2, "aPos"},
            {Enxus::ShaderDataType::Float3, "aColor"},
        };

        quadVBO->SetLayout(layout);
        m_VAO->AddVertexBuffer(quadVBO);

        int index = 0;
        float offset = 0.1f;
        for (int y = -10; y < 10; y += 2)
        {
            for (int x = -10; x < 10; x += 2)
            {
                glm::vec2 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                m_QuadTranslations[index++] = translation;
            }
        }

        Enxus::Ref<Enxus::VertexBuffer> instanceBuffer =
            Enxus::CreateRef<Enxus::VertexBuffer>(
                &m_QuadTranslations[0],
                m_QuadTranslations.size() * sizeof(glm::vec2));

        Enxus::BufferLayout instanceLayout = {
            {Enxus::ShaderDataType::Float2, "aOffset"},
        };
        instanceBuffer->SetLayout(instanceLayout);
        m_VAO->AddVertexBuffer(instanceBuffer);

        // I know exactly what vertex location is
        GLCall(glVertexAttribDivisor(2, 1));

        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-opengl/instancing/instancing-quads.vert",
                                                   "Sandbox/res/shaders/advanced-opengl/instancing/instancing-quads.frag");
    }

    TestInstancing::~TestInstancing()
    {
    }

    void TestInstancing::OnUpdate(Enxus::Camera &camera)
    {
        glm::mat4 ortho = glm::ortho(-1.0f * camera.GetAspectRatio(), 1.0f * camera.GetAspectRatio(), -1.0f, 1.0f, -1.0f, 1.0f);
        m_Shader->Bind();
        m_Shader->SetMat4("uProj", ortho);
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
    };

    void TestInstancing::OnImGuiRender()
    {
    }
}
