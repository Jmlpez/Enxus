#include "HelloBoxLayer.h"
#include "Renderer.h"
#include "Application.h"
#include "Input.h"
#include "KeyCodes.h"
#include "imgui/imgui.h"

HelloBoxLayer::HelloBoxLayer()
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
        {"Sandbox/res/images/container.jpg", Enxus::TextureType::DIFFUSE},
        {"Sandbox/res/images/awesomeface.png", Enxus::TextureType::DIFFUSE},
    };

    m_Plane = Enxus::CreateRef<Enxus::Mesh>(vertices, indices, textures);
    m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/mesh/basic.vert", "Sandbox/res/shaders/mesh/basic.frag");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(3.0f));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_Shader->SetMat4("uModel", model);

    //----------------- RENDERER -------------------//
    m_Renderer = Enxus::CreateRef<Enxus::Renderer>();

    // //----------------- CAMERA -------------------//
    auto &window = Enxus::Application::Get().GetWindow();
    m_CameraController = Enxus::CreateScope<Enxus::FreeCameraController>((float)window.GetWidth() / (float)window.GetHeight());
}

HelloBoxLayer::~HelloBoxLayer()
{
}

void HelloBoxLayer::OnUpdate(Enxus::Timestep ts)
{
    if (Enxus::Input::IsKeyPressed(Enxus::Key::Escape))
        Enxus::Application::Get().Close();
    m_CameraController->OnUpdate(ts);

    m_Shader->SetMat4("uView", m_CameraController->GetCamera().GetViewMatrix());
    m_Shader->SetMat4("uProj", m_CameraController->GetCamera().GetProjectionMatrix());

    m_Renderer->ClearColor(0.13f, 0.13f, 0.14f, 1.0f);
    m_Renderer->DrawMesh(m_Plane, m_Shader);
}
void HelloBoxLayer::OnImGuiRender()
{
    ImGui::Begin("Example");
    ImGui::Text("Example Text in a floating window");
    ImGui::End();
}

void HelloBoxLayer::OnEvent(Enxus::Event &event)
{
}