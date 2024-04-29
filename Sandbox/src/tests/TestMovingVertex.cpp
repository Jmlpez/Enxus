#include "pch.h"
#include "TestMovingVertex.h"

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

        GLCall(glGenVertexArrays(1, &m_VertexArrayID));

        GLCall(glGenBuffers(1, &m_VertexBufferID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Enxus::VertexData), nullptr, GL_DYNAMIC_DRAW));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(Enxus::VertexData), &m_Vertices[0]));

        GLCall(glBindVertexArray(m_VertexArrayID));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        GLCall(glBindVertexArray(m_VertexArrayID));
        GLCall(glGenBuffers(1, &m_IndexBufferID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

        m_Container = Enxus::CreateRef<Enxus::Texture2D>("Sandbox/res/images/container.jpg", Enxus::TextureType::DIFFUSE);
        m_AwesomeFace = Enxus::CreateRef<Enxus::Texture2D>("Sandbox/res/images/awesomeface.png", Enxus::TextureType::DIFFUSE);

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

        GLCall(glBindVertexArray(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        glDeleteVertexArrays(1, &m_VertexArrayID);
        glDeleteBuffers(1, &m_VertexBufferID);
        glDeleteBuffers(1, &m_IndexBufferID);
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
            Enxus::VertexData newVertex = m_Vertices[0];
            newVertex.Position.x = std::abs(glm::sin(glfwGetTime())) * 3.0f;
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Enxus::VertexData), &newVertex));

            newVertex = m_Vertices[1];
            newVertex.Position.x = std::abs(glm::sin(glfwGetTime())) * 3.0f;
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, sizeof(Enxus::VertexData), sizeof(Enxus::VertexData), &newVertex));

            GLCall(glBindVertexArray(m_VertexArrayID));
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        }

        // Enxus::Renderer::DrawMesh(m_Plane, m_Shader);
    }

    void TestMovingVertex::OnImGuiRender(){

    };

}
