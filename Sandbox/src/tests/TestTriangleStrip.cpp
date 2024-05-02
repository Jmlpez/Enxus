#include "pch.h"
#include "TestTriangleStrip.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestTriangleStrip::TestTriangleStrip()
    {
        /*
        Remember that the negative Z-Axis is pointing towards the viewer
        */
        m_Vertices = std::vector<Enxus::VertexData>{
            {glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // bottom right
            {glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // top right
            {glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // top left
            {glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // bottom left
        };
        unsigned int indices[4] = {0, 3, 1, 2};

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
            "Sandbox/res/shaders/advanced-glsl/point.vert",
            "Sandbox/res/shaders/advanced-glsl/point.frag");

        m_Shader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(3.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Shader->SetMat4("uModel", model);

        GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
    }

    TestTriangleStrip::~TestTriangleStrip()
    {
    }

    void TestTriangleStrip::OnUpdate(Enxus::Camera &camera)
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

            m_VertexArray->Bind();
            m_IndexBuffer->Bind();
            // Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE);
            if (m_IsWireFrame)
                Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE);
            else
                Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);

            m_Shader->SetMat4("uModel", glm::mat4(1.0f));

            GLCall(glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0));

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
            m_Shader->SetMat4("uModel", model);
            GLCall(glDrawElements(GL_POINTS, 4, GL_UNSIGNED_INT, 0));
        }
    }

    void TestTriangleStrip::OnImGuiRender()
    {
        ImGui::Checkbox("Wireframe", &m_IsWireFrame);
    };

}
