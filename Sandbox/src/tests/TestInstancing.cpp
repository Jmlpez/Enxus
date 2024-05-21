#include "pch.h"
#include "TestInstancing.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestInstancing::TestInstancing()
    {
        // float quadVertices[] = {

        //     -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        //     0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        //     -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

        //     -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        //     0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        //     0.05f, 0.05f, 0.0f, 1.0f, 1.0f};

        // m_VAO = Enxus::CreateRef<Enxus::VertexArray>();
        // m_VBO = Enxus::CreateRef<Enxus::VertexBuffer>(quadVertices, sizeof(quadVertices));

        // Enxus::VertexBufferLayout layout;
        // layout.Push(2, GL_FLOAT); // 0 --> position
        // layout.Push(3, GL_FLOAT); // 1 --> color
        // // layout.Push(2, GL_FLOAT); // 2 --> offset

        // m_VAO->AddBuffer(m_VBO, layout);

        // // m_VAO->Bind();

        // int index = 0;
        // float offset = 0.1f;
        // for (int y = -10; y < 10; y += 2)
        // {
        //     for (int x = -10; x < 10; x += 2)
        //     {
        //         glm::vec2 translation;
        //         translation.x = (float)x / 10.0f + offset;
        //         translation.y = (float)y / 10.0f + offset;
        //         m_QuadTranslations[index++] = translation;
        //     }
        // }

        // // m_InstancedBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(I)

        // unsigned int instanceVBO;
        // glGenBuffers(1, &instanceVBO);
        // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &m_QuadTranslations[0],
        //              GL_STATIC_DRAW);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // glEnableVertexAttribArray(2);
        // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glVertexAttribDivisor(2, 1);

        // m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-opengl/instancing/instancing-quads.vert",
        //                                            "Sandbox/res/shaders/advanced-opengl/instancing/instancing-quads.frag");
    }

    TestInstancing::~TestInstancing()
    {
    }

    void TestInstancing::OnUpdate(Enxus::Camera &camera) {
        // (void)camera;
        // GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
    };

    void TestInstancing::OnImGuiRender()
    {
    }

}
