#include "pch.h"
#include "EditorLayer.h"
#include "TerrainDimensionPanel.h"
#include "SceneCompositionPanel.h"
#include "TerrainBiomePanel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

EditorLayer::EditorLayer()
    : Layer("Terrain Generator Editor Layer"),
      m_ViewportSize(0.0f, 0.0f)
{

    //----------------- CAMERA -------------------//
    auto &window = Enxus::Application::Get().GetWindow();
    unsigned int width = window.GetWidth();
    unsigned int height = window.GetHeight();

    m_CameraController = Enxus::CreateScope<Enxus::FreeCameraController>((float)width / (float)height, 0.1f, 100.0f);

    // TerrainScene::Init();

    // TerrainScene::OnUpdate();

    //----------------- SHADER -------------------//
    // m_Shader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/model/box.vert", "TerrainGen/assets/shaders/model/box.frag");
    // m_TerrainShader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/terrain/terrain.vert", "TerrainGen/assets/shaders/terrain/terrain.frag");

    //----------------- BOX MODEL -------------------//

    // m_Box = Enxus::CreateRef<Enxus::Model>("TerrainGen/assets/models/box/box.obj");

    // glm::mat4 model = glm::mat4(1.0f);
    // m_Shader->Bind();
    // m_Shader->SetMat4("uModel", model);

    //----------------- TERRAIN -------------------//

    // m_TerrainMesh = Enxus::CreateScope<TerrainMesh>(m_TerrainWidth, m_TerrainHeight);

    // m_LightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);

    // m_TerrainShader->Bind();
    // glm::mat4 terrainModel = glm::mat4(1.0f);
    // m_TerrainShader->SetMat4("uModel", terrainModel);
    // m_TerrainShader->SetInt("uNumOfColors", m_NumOfBiomeLayers);
    // m_TerrainShader->SetInt("uTerrainTextures[0]", 0);

    // m_TerrainMesh->GetGrassTexture()->Bind();
    // m_TerrainShader->SetVec3("uDirLight.direction", m_LightDirection);
    // m_TerrainShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
    // m_TerrainShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
    // m_TerrainShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);

    // //----------------- TERRAIN TEXTURES -------------------//
    // static const std::string texturesPaths[7] = {
    //     "TerrainGen/assets/images/materials-debug/water.png",
    //     "TerrainGen/assets/images/materials-debug/grass.png",
    //     "TerrainGen/assets/images/materials-debug/rocks1.png",
    //     "TerrainGen/assets/images/materials-debug/rocks2.png",
    //     "TerrainGen/assets/images/materials-debug/sandy-grass.png",
    //     "TerrainGen/assets/images/materials-debug/stony-ground.png",
    //     "TerrainGen/assets/images/materials-debug/snow.png",
    // };

    // for (int i = 0; i < 7; i++)
    // {
    //     m_TexturesList[i] = Enxus::CreateRef<Enxus::TextureMesh2D>(texturesPaths[i], Enxus::TextureType::DIFFUSE);
    // }

    // //----------------- Sky Box -------------------//
    // m_SkyBox = Enxus::CreateRef<Enxus::SkyBox>();

    // m_SkyBox->SetCubeMapFaces(
    //     {"TerrainGen/assets/images/skybox/right.tga",
    //      "TerrainGen/assets/images/skybox/left.tga",
    //      "TerrainGen/assets/images/skybox/top.tga",
    //      "TerrainGen/assets/images/skybox/bottom.tga",
    //      "TerrainGen/assets/images/skybox/back.tga",
    //      "TerrainGen/assets/images/skybox/front.tga"});

    // m_SkyBoxShader = Enxus::CreateRef<Enxus::Shader>(
    //     "TerrainGen/assets/shaders/skybox/skybox.vert",
    //     "TerrainGen/assets/shaders/skybox/skybox.frag");

    m_Scene = Enxus::CreateScope<TerrainScene>();
    m_Scene->Init();
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

    // m_TerrainDimensionPanel = Enxus::CreateScope<TerrainDimensionPanel>();
    // Terrain Panels initialization

    TerrainDimensionPanel::Init();
    TerrainBiomePanel::Init();
    SceneCompositionPanel::Init();

    m_NoiseEditorPanel = Enxus::CreateScope<NoiseEditorPanel>();
    m_NoiseEditorPanel->SetNoiseWidth(250);
    m_NoiseEditorPanel->SetNoiseHeight(250);

    // Initial values
    m_Scene->UpdateTerrainNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
    m_Scene->UpdateSceneComposition(SceneCompositionPanel::GetPanelProps());
    m_Scene->UpdateTerrainDimensions(TerrainDimensionPanel::GetPanelProps());
    m_Scene->UpdateTerrainBiome(TerrainBiomePanel::GetPanelProps());

    ImGui::SetWindowFocus("Viewport");
}

void EditorLayer::OnUpdate(Enxus::Timestep ts)
{
    if (Enxus::Input::IsKeyPressed(Enxus::Key::Escape))
        Enxus::Application::Get().Close();

    if (m_IsViewportFocused)
    {
        m_CameraController->OnUpdate(ts);
    }
    HandleViewportResize();

    m_Scene->SubmitCamera(m_CameraController->GetCamera());

    {
        // Rendering
        m_Framebuffer->Bind();
        Enxus::Renderer::ClearColor(0.13f, 0.13f, 0.14f, 1.0f);
        Enxus::Renderer::Clear();

        m_Scene->OnUpdate();

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

    TerrainMenuUI();

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
    // FPS window sidebar
    {

        ImGui::BeginViewportSideBar("status", ImGui::GetMainViewport(), ImGuiDir_Down, 32, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        float textOffset = 0;
        ImGui::Text("App average %.3f ms/frame", Enxus::Application::Get().GetTimestep().GetMiliseconds());
        ImGui::SameLine(textOffset += 500);
        ImGui::Text("App average (%.1f FPS)", 1.0f / Enxus::Application::Get().GetTimestep());
        ImGui::End();
    }
    m_NoiseEditorPanel->OnImGuiRender();
    if (m_NoiseEditorPanel->HasUpdated())
    {
        // m_TerrainMesh->SetNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
        m_Scene->UpdateTerrainNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
    }

    ImGui::End();
}

void EditorLayer::TerrainMenuUI()
{

    // static const char *enumTerrainAnimationCurve[] = {"Linear", "EaseInQuad", "EaseInCubic", "EaseInQuart", "EaseInQuint"};

    //// ImGui::ShowDemoWindow();

    // Menu
    ImGui::Begin("Menu");

    ImGui::BeginTabBar("Terrain Menu TabBar");

    // int numberOfButtons = 4; // Assuming this is the input value from 1 to 8

    // ImGui::Checkbox("Grid Floor", &m_ShowGridFloor);
    ImGui::PushItemWidth(120);

    TerrainDimensionPanel::OnImGuiRender();
    auto dimensionProps = TerrainDimensionPanel::GetPanelProps();

    m_Scene->UpdateTerrainDimensions(dimensionProps);
    if ((uint32_t)dimensionProps.Width != m_NoiseEditorPanel->GetNoiseWidth())
    {
        m_NoiseEditorPanel->SetNoiseWidth(dimensionProps.Width);
        m_Scene->UpdateTerrainNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
    }
    if ((uint32_t)dimensionProps.Height != m_NoiseEditorPanel->GetNoiseHeight())
    {
        m_NoiseEditorPanel->SetNoiseHeight(dimensionProps.Height);
        m_Scene->UpdateTerrainNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
    }

    TerrainBiomePanel::OnImGuiRender();
    m_Scene->UpdateTerrainBiome(TerrainBiomePanel::GetPanelProps());

    SceneCompositionPanel::OnImGuiRender();
    m_Scene->UpdateSceneComposition(SceneCompositionPanel::GetPanelProps());

    ImGui::PopItemWidth();
    ImGui::EndTabBar();

    ImGui::End();
    // ImGui::ShowDemoWindow();
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