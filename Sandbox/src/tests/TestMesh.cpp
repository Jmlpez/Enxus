#include "pch.h"
#include "TestMesh.h"

namespace OpenGLTest
{
    TestMesh::TestMesh()
    {
        std::vector<Enxus::VertexData> vertices{
            {glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // top right
            {glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // bottom right
            {glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // bottom left
            {glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // top left
        };
        std::vector<uint32_t> indices{0, 1, 3,
                                          1, 2, 3};

        std::vector<Enxus::TextureMesh2DData> textures{
            {"Sandbox/res/images/container.jpg", Enxus::TextureType::DIFFUSE},
            {"Sandbox/res/images/awesomeface.png", Enxus::TextureType::DIFFUSE},
        };

        m_Plane = Enxus::CreateRef<Enxus::Mesh>(vertices, indices, textures);
        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/mesh/basic.vert", "Sandbox/res/shaders/mesh/basic.frag");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(3.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Shader->SetMat4("uModel", model);
    }

    TestMesh::~TestMesh()
    {
    }

    void TestMesh::OnUpdate(Enxus::Camera &camera)
    {
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetMat4("uView", camera.GetViewMatrix());

        Enxus::Renderer::DrawMesh(m_Plane, m_Shader);
    }

    void TestMesh::OnImGuiRender(){

    };

}
