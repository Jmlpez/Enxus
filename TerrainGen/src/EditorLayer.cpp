#include "pch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"

EditorLayer::EditorLayer()
    : Layer("Terrain Generator Editor Layer"), m_ViewportSize(0.0f, 0.0f)
{

    //----------------- CAMERA -------------------//

    auto &window = Enxus::Application::Get().GetWindow();
    unsigned int width = window.GetWidth();
    unsigned int height = window.GetHeight();

    m_CameraController = Enxus::CreateScope<Enxus::FreeCameraController>((float)width / (float)height);

    //----------------- SHADER -------------------//
    m_Shader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/model/box.vert", "TerrainGen/assets/shaders/model/box.frag");
    m_GridShader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/editor-grid/grid.vert", "TerrainGen/assets/shaders/editor-grid/grid.frag");
    m_TerrainShader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/heightmap/heightmap.vert", "TerrainGen/assets/shaders/heightmap/heightmap.frag");

    //----------------- BOX MODEL -------------------//

    m_Box = Enxus::CreateRef<Enxus::Model>("TerrainGen/assets/models/box/box.obj");

    glm::mat4 model = glm::mat4(1.0f);
    m_Shader->Bind();
    m_Shader->SetMat4("uModel", model);
    //----------------- GRID FLOOR -------------------//
    m_GridFloor = Enxus::CreateScope<Grid>(50, 50, 1.0f);
    m_GridShader->Bind();

    m_GridShader->SetMat4("uModel", m_GridFloor->GetModel());

    //----------------- TERRAIN -------------------//
    m_TerrainMesh = Enxus::CreateScope<TerrainMesh>(250, 250);
    m_TerrainShader->Bind();
    m_TerrainShader->SetMat4("uModel", glm::mat4(1.0f));

    //----------------- Noise Texture -------------------//
    // m_NoiseTexture = Enxus::CreateScope<Enxus::Texture2D>(1, 1);
    // uint32_t whiteTex = 0xffffffff;
    // m_NoiseTexture->SetData(&whiteTex, 1, 1);
    //----------------- Noise Generator -------------------//
    m_NoiseMapGen = Enxus::CreateScope<NoiseMapGenerator>(500, 500);
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
    //----------------- FRAMEBUFFER -------------------//
    Enxus::FramebufferSpecification fbspec;
    fbspec.Width = 800;
    fbspec.Height = 600;
    m_Framebuffer = Enxus::CreateScope<Enxus::Framebuffer>(fbspec);
    ImGui::SetWindowFocus("Viewport");
}

void EditorLayer::OnUpdate(Enxus::Timestep ts)
{
    if (Enxus::Input::IsKeyPressed(Enxus::Key::Escape))
        Enxus::Application::Get().Close();

    if (m_IsViewportFocused)
        m_CameraController->OnUpdate(ts);

    HandleViewportResize();

    m_Shader->Bind();
    m_Shader->SetMat4("uView", m_CameraController->GetCamera().GetViewMatrix());
    m_Shader->SetMat4("uProj", m_CameraController->GetCamera().GetProjectionMatrix());
    m_Shader->Unbind();

    m_GridShader->Bind();
    m_GridShader->SetMat4("uView", m_CameraController->GetCamera().GetViewMatrix());
    m_GridShader->SetMat4("uProj", m_CameraController->GetCamera().GetProjectionMatrix());
    m_GridShader->Unbind();

    m_TerrainShader->Bind();
    m_TerrainShader->SetMat4("uView", m_CameraController->GetCamera().GetViewMatrix());
    m_TerrainShader->SetMat4("uProj", m_CameraController->GetCamera().GetProjectionMatrix());
    m_TerrainShader->Unbind();

    if (m_IsWireframe)
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE);
    else
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);

    {
        // Rendering
        m_Framebuffer->Bind();
        Enxus::Renderer::ClearColor(0.13f, 0.13f, 0.14f, 1.0f);
        Enxus::Renderer::Clear();
        // Draw Grid floor
        if (m_ShowGridFloor)
        {
            Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE); // Draw the lines
            Enxus::Renderer::Draw(m_GridFloor->GetVertexArray(), m_GridFloor->GetIndexBuffer(), m_GridShader);
            Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL); // Restore state
        }
        // Draw Box
        {
            Enxus::Renderer::DrawModel(m_Box, m_Shader);

            Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL); // Draw the lines
            m_TerrainMesh->GetVertexArray()->Bind();
            m_TerrainMesh->GetIndexBuffer()->Bind();
            m_TerrainShader->Bind();
            const uint32_t numOfStrips = m_TerrainMesh->GetHeight() - 1;
            const uint32_t numOfVertPerStrip = m_TerrainMesh->GetWidth() * 2;
            for (unsigned int strip = 0; strip < numOfStrips; strip++)
            {
                size_t stripOffset = strip * numOfVertPerStrip * sizeof(unsigned int);
                glDrawElements(GL_TRIANGLE_STRIP, numOfVertPerStrip, GL_UNSIGNED_INT, (void *)stripOffset);
            }
            Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL); // Draw the lines
        }
        m_Framebuffer->Unbind();
    }
}

void EditorLayer::OnImGuiRender()
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

    {
        // Menu
        ImGui::Begin("Menu");
        ImGui::Checkbox("Grid Floor", &m_ShowGridFloor);
        ImGui::Checkbox("Wireframe Mode", &m_IsWireframe);
        // using size_t (aka unsigned long) to remove warning
        // size_t textureId = m_ExampleTexture->GetRendererId();
        ImGui::End();
    }

    {
        // Scene
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0}); // remove the window padding

        ImGui::Begin("Viewport");

        ImVec2 viewport = ImGui::GetContentRegionAvail();
        m_IsViewportFocused = ImGui::IsWindowFocused();
        m_IsViewportHovered = ImGui::IsWindowHovered();

        /*
        Stop event propagation to the EditorLayer Layer when viewport is not focused/hovered
        */
        Enxus::Application::Get().GetImGuiLayer()->BlockEvents(!m_IsViewportFocused || !m_IsViewportHovered);

        m_ViewportSize = {viewport.x, viewport.y};
        size_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image((void *)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::End();
        ImGui::PopStyleVar();
    }
    // FPS Little Window
    {
        ImGui::Begin("FPS");
        // framerate
        ImGui::Text("App average %.3f ms/frame", Enxus::Application::Get().GetTimestep().GetMiliseconds());
        ImGui::Text("App average (%.1f FPS)", 1.0f / Enxus::Application::Get().GetTimestep());
        ImGui::End();
    }

    NoiseGeneratorPanelUI();

    ImGui::End();
}

void EditorLayer::NoiseGeneratorPanelUI()
{
    // Noise Generation Properties Window
    ImGui::Begin("Noise Generator Panel");
    //
    m_NoiseMapGen->SetAmplitude(m_Amplitude);
    m_NoiseMapGen->SetLacuranity(m_Lacuranity);
    m_NoiseMapGen->SetPersistance(m_Persistance);

    {
        // Noise Texture Image
        Enxus::Ref<Enxus::Texture2D> texture = m_NoiseMapGen->GetNoiseMapTexture();
        size_t textureId = texture->GetRendererId();
        uint32_t width = texture->GetWidth();
        uint32_t height = texture->GetHeight();
        ImGui::Image((void *)textureId, ImVec2(width, height), ImVec2{0, 1}, ImVec2{1, 0});
    }

    if (ImGui::DragFloat("Frequency", &m_Frequency, 0.001f, 0.005f, 0.1f))
    {
        m_NoiseMapGen->SetFrequency(m_Frequency);
        // m_TerrainMesh->UpdateNoise()
        std::vector<std::vector<float>> heightMap;
        for (uint32_t j = 0; j < m_NoiseMapGen->GetHeight(); j++)
        {
            std::vector<float> vect;
            vect.reserve(m_NoiseMapGen->GetWidth());
            for (uint32_t i = 0; i < m_NoiseMapGen->GetHeight(); i++)
            {
                vect.emplace_back(m_NoiseMapGen->At(i, j));
            }
            heightMap.emplace_back(vect);
        }

        m_TerrainMesh->UpdateHeightFromNoise(heightMap);
    }
    ImGui::DragFloat("Amplitude", &m_Amplitude, 0.01f, 1.0f, 3.0f);
    ImGui::DragFloat("Lacuranity", &m_Lacuranity, 0.01f, 1.0f, 2.0f);
    ImGui::DragFloat("Persistance", &m_Persistance, 0.01f, 0.05f, 1.0f);

    ImGui::End();
}

void EditorLayer::HandleViewportResize()
{
    auto fbspec = m_Framebuffer->GetSpecification();
    if (m_ViewportSize.x != 0 && m_ViewportSize.y != 0 && (m_ViewportSize.x != fbspec.Width || m_ViewportSize.y != fbspec.Height))
    {
        m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        Enxus::Renderer::SetViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
    }
}

void EditorLayer::OnEvent(Enxus::Event &event)
{
    m_CameraController->OnEvent(event);
}