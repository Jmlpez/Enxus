#include "pch.h"
#include "TestInstancingV2.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestInstancingV2::TestInstancingV2()
    {

        m_Box = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/box/box.obj");

        srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
        float radius = 50.0;
        float offset = 2.5f;
        for (unsigned int i = 0; i < m_BoxInstanceMatrix.size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            // 1. translation: displace along circle with 'radius' in range [-offset, offset]
            float angle = (float)i / (float)m_BoxInstanceMatrix.size() * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. scale: Scale between 0.05 and 0.25f
            float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
            model = glm::scale(model, glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = static_cast<float>((rand() % 360));
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. now add to list of matrices
            m_BoxInstanceMatrix[i] = model;
        }
        // There is only one mesh -> The BOX
        m_Box->GetMeshes()[0]->GetVertexArray()->Bind();

        unsigned int instanceBuffer;
        glGenBuffers(1, &instanceBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_BoxInstanceMatrix.size() * sizeof(glm::mat4), &m_BoxInstanceMatrix[0], GL_STATIC_DRAW);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        std::size_t v4s = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(1 * v4s));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(2 * v4s));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * v4s, (void *)(3 * v4s));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-opengl/instancing/instancing-obj.vert",
                                                   "Sandbox/res/shaders/advanced-opengl/instancing/instancing-obj.frag");
        // m_Shader->Bind();
        // glm::mat4 model = glm::mat4(1.0f);
        // m_Shader->SetMat4("uModel", model);

        //----------------- FACE CULLING -------------------//
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        GLCall(glFrontFace(GL_CW));
    }

    TestInstancingV2::~TestInstancingV2()
    {
    }

    void TestInstancingV2::OnUpdate(Enxus::Camera &camera)
    {
        m_Shader->SetMat4("uView", camera.GetViewMatrix());
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());

        for (const auto &mesh : m_Box->GetMeshes())
        {
            // int textureIndex
            // for (const auto textures : mesh->GetTextures())
            // {
            //     m_Shader->SetInt(("material." + name).c_str(), i);
            //     m_Texture->Bind(i); // active the current texture slot
            // }
            mesh->GetTextures()[0]->Bind(0);
            m_Shader->SetInt("material.texture_diffuse1", 0);

            mesh->GetVertexArray()->Bind();
            GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, 1000));
        }

        // Enxus::Renderer::DrawModel(m_Box, m_Shader);
    };

    void TestInstancingV2::OnImGuiRender()
    {
    }

}
