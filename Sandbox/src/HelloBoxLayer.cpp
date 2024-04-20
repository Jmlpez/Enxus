#include "HelloBoxLayer.h"
#include "Renderer.h"
#include "Application.h"
#include "Input.h"
#include "KeyCodes.h"
#include "imgui/imgui.h"

HelloBoxLayer::HelloBoxLayer()
    : Layer("HelloBox Layer")
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

    // //----------------- CAMERA -------------------//
    auto &window = Enxus::Application::Get().GetWindow();
    m_CameraController = Enxus::CreateScope<Enxus::FreeCameraController>((float)window.GetWidth() / (float)window.GetHeight());

    //----------------- EXAMPLE TEXTURE -------------------//
    m_ExampleTexture = Enxus::CreateRef<Enxus::Texture2D>("Sandbox/res/images/container.jpg", Enxus::TextureType::DIFFUSE);
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

    Enxus::Renderer::ClearColor(0.13f, 0.13f, 0.14f, 1.0f);
    Enxus::Renderer::DrawMesh(m_Plane, m_Shader);
}
void HelloBoxLayer::OnImGuiRender()
{

    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

            if (ImGui::MenuItem("Exit"))
                Enxus::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Example");
    ImGui::Text("Example Text in a floating window");

    unsigned int textureId = m_ExampleTexture->GetRendererId();
    ImGui::Image((void *)textureId, ImVec2(256.0f, 256.0f));
    ImGui::End();

    ImGui::End();
}

void HelloBoxLayer::OnEvent(Enxus::Event &event)
{
    m_CameraController->OnEvent(event);
}