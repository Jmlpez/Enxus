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
        std::vector<Enxus::VertexData> m_Vertices{
            {glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // bottom right
            {glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // top right
            {glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // top left
            {glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // bottom left
        };

        uint32_t indices[4] = {0, 3, 1, 2};

        m_VertexArray = Enxus::CreateRef<Enxus::VertexArray>();

        Enxus::Ref<Enxus::VertexBuffer> vertexBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Enxus::VertexData));

        Enxus::BufferLayout layout = {
            {Enxus::ShaderDataType::Float3, "aPos"},
            {Enxus::ShaderDataType::Float2, "aTexCoord"},
            {Enxus::ShaderDataType::Float3, "aNormal"},
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        Enxus::Ref<Enxus::IndexBuffer> indexBuffer = Enxus::CreateRef<Enxus::IndexBuffer>(indices, 4);
        m_VertexArray->SetIndexBuffer(indexBuffer);

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
            // m_IndexBuffer->Bind();
            //  Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE);
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
