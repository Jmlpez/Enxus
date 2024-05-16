#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
}

TerrainScene::~TerrainScene()
{
}

void TerrainScene::SubmitCamera(const Enxus::Camera &camera)
{
    m_CameraData.ViewMatrix = camera.GetViewMatrix();
    m_CameraData.ProjectionMatrix = camera.GetProjectionMatrix();
    m_CameraData.Position = camera.GetPos();
}

void TerrainScene::Init()
{
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

    m_TerrainShader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/terrain/terrain.vert",
                                                      "TerrainGen/assets/shaders/terrain/terrain.frag");

    // Default terrain
    m_TerrainMesh = Enxus::CreateScope<TerrainMesh>(250, 250);

    // m_SceneCompositionData.LightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
    // m_SceneCompositionData.IsWireframe = false;

    m_TerrainShader->Bind();
    glm::mat4 terrainModel = glm::mat4(1.0f);
    m_TerrainShader->SetMat4("uModel", terrainModel);

    m_TerrainShader->SetVec3("uDirLight.direction", m_SceneCompositionData.LightDirection);
    m_TerrainShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
    m_TerrainShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
    m_TerrainShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);

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

void TerrainScene::OnUpdate()
{

    if (m_SceneCompositionData.IsWireframe)
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE);
    else
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);

    {
        m_TerrainShader->Bind();

        m_TerrainShader->SetMat4("uView", m_CameraData.ViewMatrix);
        m_TerrainShader->SetMat4("uProj", m_CameraData.ProjectionMatrix);

        // m_TerrainMesh->GetGrassTexture()->Bind(0);
        m_TerrainShader->SetVec3("uCameraPos", m_CameraData.Position);
        m_TerrainShader->SetVec3("uDirLight.direction", m_SceneCompositionData.LightDirection);
        m_TerrainShader->SetFloat("uMinHeight", m_TerrainMesh->GetMinHeight());
        m_TerrainShader->SetFloat("uMaxHeight", m_TerrainMesh->GetMaxHeight());

        m_TerrainShader->SetInt("uNumOfColors", m_TerrainBiomeData.NumOfBiomeLayers);

        for (int i = 0; i < m_TerrainBiomeData.NumOfBiomeLayers; i++)
        {
            bool textureUsed = false;
            // Bind Textures
            if (m_TerrainBiomeData.BiomeLayers[i].TextureIndex != 0)
            {
                m_TerrainShader->SetFloat("uTexturesScale[" + std::to_string(i) + "]", m_TerrainBiomeData.BiomeLayers[i].TextureScale);
                m_TerrainShader->SetInt("uTerrainTextures[" + std::to_string(i) + "]", i);

                // m_TerrainBiomeData.BiomeLayers[i].Texture->Bind(i);
                m_TexturesList[m_TerrainBiomeData.BiomeLayers[i].TextureIndex - 1]->Bind(i);

                textureUsed = true;
            }
            m_TerrainShader->SetBool("uBiomeTextureUsed[" + std::to_string(i) + "]", textureUsed);

            m_TerrainShader->SetFloat("uBiomeStartHeight[" + std::to_string(i) + "]", m_TerrainBiomeData.BiomeLayers[i].StartHeight);
            m_TerrainShader->SetFloat("uBiomeBlends[" + std::to_string(i) + "]", m_TerrainBiomeData.BiomeLayers[i].BlendStrength);
            m_TerrainShader->SetFloat("uBiomeColorStrength[" + std::to_string(i) + "]", m_TerrainBiomeData.BiomeLayers[i].ColorStrength);
            m_TerrainShader->SetVec3("uBiomeColors[" + std::to_string(i) + "]", m_TerrainBiomeData.BiomeLayers[i].Color);
        }
        // pass the index of the textures used

        // m_TerrainShader->SetInt()
        m_TerrainMesh->GetVertexArray()->Bind();
        m_TerrainMesh->GetIndexBuffer()->Bind();

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

        glm::mat4 viewMatrix = glm::mat4(glm::mat3(m_CameraData.ViewMatrix));

        m_SkyBoxShader->Bind();
        m_SkyBoxShader->SetMat4("uView", viewMatrix);
        m_SkyBoxShader->SetMat4("uProj", m_CameraData.ProjectionMatrix);

        m_SkyBoxShader->SetInt("uSkyBoxTexture", 0);
        m_SkyBox->Bind();

        m_SkyBox->GetVertexArray()->Bind();

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

        GLCall(glDepthMask(GL_TRUE)); // Restoring state
        GLCall(glDepthFunc(GL_LESS)); // Restoring state
    }
}

void TerrainScene::UpdateTerrainNoiseMap(const std::vector<float> &noiseMap)
{
    m_TerrainMesh->SetNoiseMap(noiseMap);
}

void TerrainScene::UpdateTerrainDimensions(const TerrainDimensionPanelProps &props)
{
    // update the terrain according to the new panel props...
    if ((uint32_t)props.Width != m_TerrainMesh->GetWidth())
    {
        m_TerrainMesh->SetWidth(props.Width);
    }
    if ((uint32_t)props.Height != m_TerrainMesh->GetHeight())
    {
        m_TerrainMesh->SetHeight(props.Height);
    }
    if (props.Elevation != m_TerrainMesh->GetElevation())
    {
        m_TerrainMesh->SetElevation(props.Elevation);
    }
    if (props.VertexScale != m_TerrainMesh->GetVertexDistance())
    {
        m_TerrainMesh->SetVertexDistance(props.VertexScale);
    }
    if (props.ElevationCurve != (int)m_TerrainMesh->GetHeightElevationCurve())
    {
        m_TerrainMesh->SetHeightElevationCurve((AnimationCurve)props.ElevationCurve);
    }
}

void TerrainScene::UpdateTerrainBiome(const TerrainBiomePanelProps &props)
{
    m_TerrainBiomeData = props;
}

void TerrainScene::UpdateSceneComposition(const SceneCompositionPanelProps &props)
{
    m_SceneCompositionData = props;
    // m_SceneCompositionData.IsWireframe = props.IsWireframe;
    // if (props.LightDirection != m_SceneCompositionData.LightDirection)
    // {
    //     m_SceneCompositionData.LightDirection = props.LightDirection;
    // }
}
