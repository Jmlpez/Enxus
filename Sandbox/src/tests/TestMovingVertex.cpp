#include "pch.h"
#include "TestMovingVertex.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestMovingVertex::TestMovingVertex()
    {
        m_Vertices = std::vector<Enxus::VertexData>{
            {glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // top right
            {glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // bottom right
            {glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // bottom left
            {glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // top left
        };
        unsigned int indices[6] = {0, 1, 3,
                                   1, 2, 3};

        m_VertexArray = Enxus::CreateRef<Enxus::VertexArray>();

        m_VertexBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(sizeof(m_Vertices) * sizeof(Enxus::VertexData));
        m_VertexBuffer->SetData(&m_Vertices[0], sizeof(m_Vertices) * sizeof(Enxus::VertexData), 0);

        Enxus::VertexBufferLayout layout;
        layout.Push(3, GL_FLOAT); // position
        layout.Push(2, GL_FLOAT); // tex coords
        layout.Push(3, GL_FLOAT); // normals
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = Enxus::CreateRef<Enxus::IndexBuffer>(&indices[0], sizeof(indices));

        m_Container = Enxus::CreateRef<Enxus::TextureMesh2D>("Sandbox/res/images/container.jpg", Enxus::TextureType::DIFFUSE);
        m_AwesomeFace = Enxus::CreateRef<Enxus::TextureMesh2D>("Sandbox/res/images/awesomeface.png", Enxus::TextureType::DIFFUSE);

        // m_Plane = Enxus::CreateRef<Enxus::Mesh>(vertices, indices, textures);
        m_Shader = Enxus::CreateRef<Enxus::Shader>(
            "Sandbox/res/shaders/advanced-opengl/moving-vertex/basic.vert",
            "Sandbox/res/shaders/advanced-opengl/moving-vertex/basic.frag");

        m_Shader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(3.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Shader->SetMat4("uModel", model);
    }

    TestMovingVertex::~TestMovingVertex()
    {
    }

    void TestMovingVertex::OnUpdate(Enxus::Camera &camera)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetMat4("uView", camera.GetViewMatrix());

        // Draw the object
        {
            m_Shader->SetInt("uContainer", 0);
            m_Container->Bind(0);

            m_Shader->SetInt("uFace", 1);
            m_AwesomeFace->Bind(1);

            // Modify the first(0) and the second vertex(1) in the vertices array to create an animation
            /*
             (3)*-----*(0)
                |     |
                |     |
             (2)*-----*(1)
            */
            if (m_IsAnimatingVertices)
            {
                Enxus::VertexData newVertex = m_Vertices[0];
                newVertex.Position.x = std::abs(glm::sin(glfwGetTime() / m_WaveSpeed)) * m_DistanceFactor;
                m_VertexBuffer->SetData(&newVertex, sizeof(Enxus::VertexData), 0);

                newVertex = m_Vertices[1];
                newVertex.Position.x = std::abs(glm::sin(glfwGetTime() / m_WaveSpeed)) * m_DistanceFactor;
                m_VertexBuffer->SetData(&newVertex, sizeof(Enxus::VertexData), sizeof(Enxus::VertexData));
            }

            Enxus::Renderer::Draw(m_VertexArray, m_IndexBuffer, m_Shader);
        }
    }

    void TestMovingVertex::OnImGuiRender()
    {
        ImGui::DragFloat("Speed", &m_WaveSpeed, 0.1f, 0.1f, 5.0f);
        ImGui::DragFloat("Distance Factor", &m_DistanceFactor, 0.05f, 0.1f, 5.0f);
        ImGui::Checkbox("Animate Vertices", &m_IsAnimatingVertices);
    };

}
