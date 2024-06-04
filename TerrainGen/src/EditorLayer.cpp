#include "pch.h"
#include "EditorLayer.h"
#include "NoiseEditorPanel.h"
#include "TerrainDimensionPanel.h"
#include "SceneCompositionPanel.h"
#include "TerrainBiomePanel.h"
#include "ModelPlacementPanel.h"
#include "TerrainScene.h"
#include "ResourceManager.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

EditorLayer::EditorLayer()
    : Layer("Terrain Generator Editor Layer"),
      m_ViewportSize(0.0f, 0.0f)
{
    //----------------- CAMERA -------------------//
    auto &window = Enxus::Application::Get().GetWindow();
    uint32_t width = window.GetWidth();
    uint32_t height = window.GetHeight();

    m_CameraController = Enxus::CreateScope<Enxus::FreeCameraController>((float)width / (float)height, 0.1f, 100.0f);
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

    //-----------------  Resource Manager Initialization -------------------//
    ResourceManager::Init();

    //----------------- Terrain Panels Initialization -------------------//

    TerrainDimensionPanel::Init();
    TerrainBiomePanel::Init();
    SceneCompositionPanel::Init();
    ModelPlacementPanel::Init();
    NoiseEditorPanel::Init();

    //----------------- Terrain Scene Initialization -------------------//

    TerrainScene::Init();

    // Initial values
    TerrainScene::UpdateTerrainNoiseMap(NoiseEditorPanel::GetNoiseMap());
    TerrainScene::UpdateSceneComposition(SceneCompositionPanel::GetPanelProps());
    TerrainScene::UpdateTerrainDimensions(TerrainDimensionPanel::GetPanelProps());
    TerrainScene::UpdateTerrainBiome(TerrainBiomePanel::GetPanelProps());
    TerrainScene::UpdateModelPlacement(ModelPlacementPanel::GetPanelProps());

    ImGui::SetWindowFocus("Viewport");
}

void EditorLayer::OnDetach()
{
    //----------------- Terrain Scene ShutDown -------------------//
    TerrainScene::ShutDown();

    //----------------- Terrain Panels ShutDown -------------------//

    TerrainDimensionPanel::ShutDown();
    TerrainBiomePanel::ShutDown();
    SceneCompositionPanel::ShutDown();
    ModelPlacementPanel::ShutDown();
    NoiseEditorPanel::ShutDown();

    //----------------- Resource Manager ShutDown -------------------//
    ResourceManager::ShutDown();
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

    TerrainScene::SubmitCamera(m_CameraController->GetCamera());

    {
        // Shadow pass
        TerrainScene::OnShadowPass();
        Enxus::Renderer::SetViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
        Enxus::Renderer::Clear();
    }

    {
        // Main Render Pass
        m_Framebuffer->Bind();
        Enxus::Renderer::ClearColor(0.13f, 0.13f, 0.14f, 1.0f);
        Enxus::Renderer::Clear();
        TerrainScene::OnRenderPass();

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
            if (ImGui::MenuItem("Exit"))
                Enxus::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    TerrainMenuUI();

    {
        // Viewport
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

        ImGui::BeginViewportSideBar("status", ImGui::GetMainViewport(), ImGuiDir_Up, 24, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Text("App average %.3f ms/frame (%.1f FPS)", Enxus::Application::Get().GetTimestep().GetMiliseconds(), 1.0f / Enxus::Application::Get().GetTimestep());
        ImGui::End();
    }
    // The Noise is updated separately
    NoiseEditorPanel::OnImGuiRender();
    if (NoiseEditorPanel::HasUpdated())
    {
        TerrainScene::UpdateTerrainNoiseMap(NoiseEditorPanel::GetNoiseMap());
    }

    ImGui::End();
}

void EditorLayer::TerrainMenuUI()
{
    // Menu
    {
        ImGui::Begin("Terrain Menu");
        ImGui::BeginTabBar("Terrain Menu TabBar");

        TerrainDimensionPanel::OnImGuiRender();
        auto dimensionProps = TerrainDimensionPanel::GetPanelProps();
        TerrainScene::UpdateTerrainDimensions(dimensionProps);

        TerrainBiomePanel::OnImGuiRender();
        TerrainScene::UpdateTerrainBiome(TerrainBiomePanel::GetPanelProps());

        SceneCompositionPanel::OnImGuiRender();
        TerrainScene::UpdateSceneComposition(SceneCompositionPanel::GetPanelProps());

        ModelPlacementPanel::OnImGuiRender();
        TerrainScene::UpdateModelPlacement(ModelPlacementPanel::GetPanelProps());

        ImGui::EndTabBar();
        ImGui::End();
    }
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