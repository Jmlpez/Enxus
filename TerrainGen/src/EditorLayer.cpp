#include "pch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

EditorLayer::EditorLayer()
    : Layer("Terrain Generator Editor Layer"),
      m_ViewportSize(0.0f, 0.0f),
      m_TerrainWidth(250),
      m_TerrainHeight(250)
{

    //----------------- CAMERA -------------------//
    auto &window = Enxus::Application::Get().GetWindow();
    unsigned int width = window.GetWidth();
    unsigned int height = window.GetHeight();

    m_CameraController = Enxus::CreateScope<Enxus::FreeCameraController>((float)width / (float)height, 0.1f, 100.0f);

    //----------------- SHADER -------------------//
    m_Shader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/model/box.vert", "TerrainGen/assets/shaders/model/box.frag");
    m_TerrainShader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/terrain/terrain.vert", "TerrainGen/assets/shaders/terrain/terrain.frag");

    //----------------- BOX MODEL -------------------//

    m_Box = Enxus::CreateRef<Enxus::Model>("TerrainGen/assets/models/box/box.obj");

    glm::mat4 model = glm::mat4(1.0f);
    m_Shader->Bind();
    m_Shader->SetMat4("uModel", model);

    //----------------- TERRAIN -------------------//

    m_TerrainMesh = Enxus::CreateScope<TerrainMesh>(m_TerrainWidth, m_TerrainHeight);

    m_LightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);

    m_TerrainShader->Bind();
    glm::mat4 terrainModel = glm::mat4(1.0f);
    m_TerrainShader->SetMat4("uModel", terrainModel);
    m_TerrainShader->SetInt("uNumOfColors", m_NumOfBiomeLayers);
    // m_TerrainShader->SetInt("uTerrainTextures[0]", 0);

    // m_TerrainMesh->GetGrassTexture()->Bind();
    m_TerrainShader->SetVec3("uDirLight.direction", m_LightDirection);
    m_TerrainShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
    m_TerrainShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
    m_TerrainShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);

    //----------------- TERRAIN TEXTURES -------------------//
    static const std::string texturesPaths[7] = {
        "TerrainGen/assets/images/materials-debug/water.png",
        "TerrainGen/assets/images/materials-debug/grass.png",
        "TerrainGen/assets/images/materials-debug/rocks1.png",
        "TerrainGen/assets/images/materials-debug/rocks2.png",
        "TerrainGen/assets/images/materials-debug/sandy-grass.png",
        "TerrainGen/assets/images/materials-debug/stony-ground.png",
        "TerrainGen/assets/images/materials-debug/snow.png",
    };

    for (int i = 0; i < 7; i++)
    {
        m_TexturesList[i] = Enxus::CreateRef<Enxus::TextureMesh2D>(texturesPaths[i], Enxus::TextureType::DIFFUSE);
    }

    //----------------- Sky Box -------------------//
    m_SkyBox = Enxus::CreateRef<Enxus::SkyBox>();

    m_SkyBox->SetCubeMapFaces(
        {"TerrainGen/assets/images/skybox/right.tga",
         "TerrainGen/assets/images/skybox/left.tga",
         "TerrainGen/assets/images/skybox/top.tga",
         "TerrainGen/assets/images/skybox/bottom.tga",
         "TerrainGen/assets/images/skybox/back.tga",
         "TerrainGen/assets/images/skybox/front.tga"});

    m_SkyBoxShader = Enxus::CreateRef<Enxus::Shader>(
        "TerrainGen/assets/shaders/skybox/skybox.vert",
        "TerrainGen/assets/shaders/skybox/skybox.frag");
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

    m_NoiseEditorPanel = Enxus::CreateScope<NoiseEditorPanel>();
    m_NoiseEditorPanel->SetNoiseWidth(m_TerrainWidth);
    m_NoiseEditorPanel->SetNoiseHeight(m_TerrainHeight);
    m_TerrainMesh->SetNoiseMap(m_NoiseEditorPanel->GetNoiseMap());

    ImGui::SetWindowFocus("Viewport");
}

void EditorLayer::OnUpdate(Enxus::Timestep ts)
{
    if (Enxus::Input::IsKeyPressed(Enxus::Key::Escape))
        Enxus::Application::Get().Close();

    if (m_IsViewportFocused)
        m_CameraController->OnUpdate(ts);

    HandleViewportResize();

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
        // Draw Terrain
        {
            // Enxus::Renderer::DrawModel(m_Box, m_Shader);

            m_TerrainMesh->GetVertexArray()->Bind();
            m_TerrainMesh->GetIndexBuffer()->Bind();
            m_TerrainShader->Bind();
            // m_TerrainMesh->GetGrassTexture()->Bind(0);
            m_TerrainShader->SetVec3("uCameraPos", m_CameraController->GetCamera().GetPos());
            m_TerrainShader->SetVec3("uDirLight.direction", m_LightDirection);
            m_TerrainShader->SetFloat("uMinHeight", m_TerrainMesh->GetMinHeight());
            m_TerrainShader->SetFloat("uMaxHeight", m_TerrainMesh->GetMaxHeight());

            m_TerrainShader->SetInt("uNumOfColors", m_NumOfBiomeLayers);

            for (int i = 0; i < m_NumOfBiomeLayers; i++)
            {
                bool textureUsed = false;
                // Bind Textures
                if (m_BiomeLayers[i].Texture)
                {
                    m_TerrainShader->SetFloat("uTexturesScale[" + std::to_string(i) + "]", m_BiomeLayers[i].TextureScale);
                    m_TerrainShader->SetInt("uTerrainTextures[" + std::to_string(i) + "]", i);

                    m_BiomeLayers[i].Texture->Bind(i);
                    textureUsed = true;
                }
                m_TerrainShader->SetBool("uBiomeTextureUsed[" + std::to_string(i) + "]", textureUsed);

                m_TerrainShader->SetFloat("uBiomeStartHeight[" + std::to_string(i) + "]", m_BiomeLayers[i].StartHeight);
                m_TerrainShader->SetFloat("uBiomeBlends[" + std::to_string(i) + "]", m_BiomeLayers[i].BlendStrength);
                m_TerrainShader->SetFloat("uBiomeColorStrength[" + std::to_string(i) + "]", m_BiomeLayers[i].ColorStrength);
                m_TerrainShader->SetVec3("uBiomeColors[" + std::to_string(i) + "]", m_BiomeLayers[i].Color);
            }
            // pass the index of the textures used

            // m_TerrainShader->SetInt()

            const uint32_t numOfStrips = m_TerrainMesh->GetHeight() - 1;
            const uint32_t numOfVertPerStrip = m_TerrainMesh->GetWidth() * 2;
            for (unsigned int strip = 0; strip < numOfStrips; strip++)
            {
                size_t stripOffset = strip * numOfVertPerStrip * sizeof(unsigned int);
                glDrawElements(GL_TRIANGLE_STRIP, numOfVertPerStrip, GL_UNSIGNED_INT, (void *)stripOffset);
            }
        }
        // Draw Skybox at last
        {
            // always render the skybox with fill mode and not wireframe
            Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);
            GLCall(glDepthMask(GL_FALSE));
            GLCall(glDepthFunc(GL_LEQUAL));

            glm::mat4 viewMatrix = glm::mat4(glm::mat3(m_CameraController->GetCamera().GetViewMatrix()));

            m_SkyBoxShader->Bind();
            m_SkyBoxShader->SetMat4("uView", viewMatrix);
            m_SkyBoxShader->SetMat4("uProj", m_CameraController->GetCamera().GetProjectionMatrix());

            m_SkyBoxShader->SetInt("uSkyBoxTexture", 0);
            m_SkyBox->Bind();

            m_SkyBox->GetVertexArray()->Bind();

            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

            GLCall(glDepthMask(GL_TRUE)); // Restoring state
            GLCall(glDepthFunc(GL_LESS)); // Restoring state
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

        ASSERT(
            m_NoiseEditorPanel->GetNoiseWidth() == m_TerrainMesh->GetWidth() &&
            m_NoiseEditorPanel->GetNoiseHeight() == m_TerrainMesh->GetHeight())

        m_TerrainMesh->SetNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
    }

    ImGui::End();
}

void EditorLayer::TerrainMenuUI()
{

    static const char *enumTerrainAnimationCurve[] = {"Linear", "EaseInQuad", "EaseInCubic", "EaseInQuart", "EaseInQuint"};
    static const char *enumTerrainTextures[] = {"None", "Water", "Grass", "Rocks1", "Rocks2", "Sandy Grass", "Stony Ground", "Snow"};

    //// ImGui::ShowDemoWindow();

    // Menu
    ImGui::Begin("Menu");

    // int numberOfButtons = 4; // Assuming this is the input value from 1 to 8

    // ImGui::Checkbox("Grid Floor", &m_ShowGridFloor);
    ImGui::PushItemWidth(120);
    // ImGui::Swap
    ImGui::Checkbox("Wireframe Mode", &m_IsWireframe);
    ImGui::DragFloat3("Light Direction", glm::value_ptr(m_LightDirection), 0.1f);
    if (ImGui::DragFloat("Elevation", &m_TerrainElevation, 0.01f, 0.0f))
    {
        m_TerrainMesh->SetElevation(m_TerrainElevation);
    }
    if (ImGui::DragFloat("Distance", &m_VertexDistance, 0.001f, 0.01f, 2.0f))
    {
        m_TerrainMesh->SetVertexDistance(m_VertexDistance);
    }
    if (ImGui::SliderInt("Terrain Width", (int *)&m_TerrainWidth, 50, 500))
    {
        m_TerrainMesh->SetWidth(m_TerrainWidth);
        m_NoiseEditorPanel->SetNoiseWidth(m_TerrainWidth);
        m_TerrainMesh->SetNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
    }
    if (ImGui::SliderInt("Terrain Height", (int *)&m_TerrainHeight, 50, 500))
    {
        m_TerrainMesh->SetHeight(m_TerrainHeight);
        m_NoiseEditorPanel->SetNoiseHeight(m_TerrainHeight);
        m_TerrainMesh->SetNoiseMap(m_NoiseEditorPanel->GetNoiseMap());
    }

    if (ImGui::Combo("Elevation Curve", &m_TerrainElevationCurve, enumTerrainAnimationCurve, IM_ARRAYSIZE(enumTerrainAnimationCurve)))
    {
        m_TerrainMesh->SetHeightElevationCurve((AnimationCurve)m_TerrainElevationCurve);
    }

    ImGui::DragFloat("Texture Scale", &m_TextureScale, 0.01f);

    ImGui::SliderInt("Layers", &m_NumOfBiomeLayers, 1, 8);

    for (int i = 0; i < m_NumOfBiomeLayers; i++)
    {
        std::string index = std::to_string(i);
        std::string uiName = "Layer " + index;
        ImGui::SeparatorText(uiName.c_str());
        ImGui::ColorEdit3(("Color - " + index).c_str(), glm::value_ptr(m_BiomeLayers[i].Color));
        ImGui::SliderFloat(("Color Strength - " + index).c_str(), &m_BiomeLayers[i].ColorStrength, 0.0f, 1.0f);
        ImGui::SliderFloat(("Start Height - " + index).c_str(), &m_BiomeLayers[i].StartHeight, 0.0f, 1.0f);
        ImGui::SliderFloat(("Blend Strength - " + index).c_str(), &m_BiomeLayers[i].BlendStrength, 0.0f, 1.0f);
        ImGui::DragFloat(("Texture Scale - " + index).c_str(), &m_BiomeLayers[i].TextureScale, 0.1f);

        if (ImGui::Combo(("Texture - " + index).c_str(), &m_BiomeLayers[i].TextureIndex, enumTerrainTextures, IM_ARRAYSIZE(enumTerrainTextures)))
        {
            // if its the NONE texture, just release the reference
            if (m_BiomeLayers[i].TextureIndex == 0)
            {
                m_BiomeLayers[i].Texture.reset();
            }
            else
            {
                m_BiomeLayers[i].Texture = m_TexturesList[m_BiomeLayers[i].TextureIndex - 1];
            }
        }

        // ImGui::ColorEdit3("Color2", glm::value_ptr(m_BiomeColor[1]));
        // ImGui::SliderFloat("Second Start Height", &m_BiomeStartHeight[1], 0.0f, 1.0f);
        // ImGui::SliderFloat("Color2 Blend", &m_BiomeBlends[1], 0.0f, 1.0f);
    }
    ImGui::Dummy(ImVec2(0.0f, 15.0f));

    ImGui::PopItemWidth();
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