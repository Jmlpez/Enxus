#include "pch.h"
#include "TestInstancingV2.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    const uint32_t g_RenderedAmount = 50;
    TestInstancingV2::TestInstancingV2()
    {

        m_Box = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/backpack/backpack.obj");

        srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
        float radius = 12.0;
        float offset = 2.5f;
        for (unsigned int i = 0; i < g_RenderedAmount; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            // 1. translation: displace along circle with 'radius' in range [-offset, offset]
            float angle = (float)i / (float)g_RenderedAmount * 360.0f;
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

        Enxus::Ref<Enxus::VertexBuffer> instanceBuffer =
            Enxus::CreateRef<Enxus::VertexBuffer>(
                &m_BoxInstanceMatrix[0],
                g_RenderedAmount * sizeof(glm::mat4));

        Enxus::BufferLayout layout = {
            {Enxus::ShaderDataType::Mat4, "aInstanceMatrix"},
        };
        instanceBuffer->SetLayout(layout);
        m_Box->GetMeshes()[0]->GetVertexArray()->AddVertexBuffer(instanceBuffer);

        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-opengl/instancing/instancing-obj.vert", "Sandbox/res/shaders/advanced-opengl/instancing/instancing-obj.frag");
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

        // m_Shader->SetMat4("uView", camera.GetViewMatrix());
        // m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetMat4("uViewProj", camera.GetViewProjectionMatrix());

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
            GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, g_RenderedAmount));
        }

        // Enxus::Renderer::DrawModel(m_Box, m_Shader);
    };

    void TestInstancingV2::OnImGuiRender()
    {
    }

}
