#include "TestMesh.h"

namespace Test
{
    TestMesh::TestMesh()
    {
        std::vector<Enxus::VertexData> vertices{
            {glm::vec3(0.5f, 0.0f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},   // top right
            {glm::vec3(0.5f, 0.0f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // bottom right
            {glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // bottom left
            {glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // top left
        };
        std::vector<unsigned int> indices{0, 1, 3,
                                          1, 2, 3};

        std::vector<Enxus::TextureData2D> textures{
            {"res/images/container.jpg", Enxus::TextureType::DIFFUSE},
            {"res/images/awesomeface.png", Enxus::TextureType::DIFFUSE},
        };

        m_Plane = std::make_shared<Enxus::Mesh>(vertices, indices, textures);
        m_Shader = std::make_shared<Enxus::Shader>("res/shaders/mesh/basic.vert", "res/shaders/mesh/basic.frag");

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(3.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        m_Shader->SetMat4("uModel", model);
    }

    TestMesh::~TestMesh()
    {
    }

    void TestMesh::OnUpdate(float deltaTime, Enxus::FreeCameraController *cameraController)
    {
        (void)deltaTime;
        cameraController->GetCamera()->SetViewProjMatrix(*m_Shader);
    }
    void TestMesh::OnRender()
    {
        Enxus::Renderer renderer;
        renderer.Draw(m_Plane, *m_Shader);
    };

    void TestMesh::OnImGuiRender(){

    };

}
