#include "TerrainScene.h"

struct TerrainSceneData
{
    Enxus::Ref<Enxus::Model> Box;
    Enxus::Ref<TerrainMesh> Terrain;

    struct ModelPlacementProps
    {
        uint32_t Amount = 100;
        float Scale = 0.05f;
        std::vector<glm::vec2> Positions;
    } ModelPlacementData;

    SceneCompositionPanelProps SceneCompositionData;
    TerrainBiomePanelProps TerrainBiomeData;

    // SkyBox
    Enxus::Ref<Enxus::SkyBox> SkyBox;
    Enxus::Ref<Enxus::Shader> SkyBoxShader;

    // Shaders
    Enxus::Ref<Enxus::Shader> TerrainShader;
    Enxus::Ref<Enxus::Shader> ModelShader;

    std::array<Enxus::Ref<Enxus::TextureMesh2D>, 7> TexturesList;

    struct CameraData
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::vec3 Position;
    } CameraData;
};

static TerrainSceneData s_Data;

void TerrainScene::SubmitCamera(const Enxus::Camera &camera)
{
    s_Data.CameraData.ViewMatrix = camera.GetViewMatrix();
    s_Data.CameraData.ProjectionMatrix = camera.GetProjectionMatrix();
    s_Data.CameraData.Position = camera.GetPos();
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
        s_Data.TexturesList[i] = Enxus::CreateRef<Enxus::TextureMesh2D>(texturesPaths[i], Enxus::TextureType::DIFFUSE);
    }

    // [TODO] Use Uniform Buffer Objects for things like camera and lighting
    InitTerrain();
    InitModels();
    InitSkyBox();
}

void TerrainScene::ShutDown()
{
}

void TerrainScene::InitTerrain()
{
    //----------------- Default Terrain -------------------//
    s_Data.Terrain = Enxus::CreateRef<TerrainMesh>(241, 241);

    s_Data.TerrainShader = Enxus::CreateRef<Enxus::Shader>("TerrainGen/assets/shaders/terrain/terrain.vert",
                                                           "TerrainGen/assets/shaders/terrain/terrain.frag");
    s_Data.TerrainShader->Bind();
    glm::mat4 terrainModel = glm::mat4(1.0f);
    // terrainModel = glm::translate(terrainModel, glm::vec3(7.8325f, 0.0f, 0.0f));
    s_Data.TerrainShader->SetMat4("uModel", terrainModel);

    s_Data.TerrainShader->SetVec3("uDirLight.direction", s_Data.SceneCompositionData.LightDirection);
    s_Data.TerrainShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
    s_Data.TerrainShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
    s_Data.TerrainShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);
}

void TerrainScene::InitModels()
{
    s_Data.Box = Enxus::CreateRef<Enxus::Model>("TerrainGen/assets/models/box/box.obj");
    // s_Data.Box = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/backpack/backpack.obj");
    s_Data.ModelShader = Enxus::CreateRef<Enxus::Shader>(
        "TerrainGen/assets/shaders/model/model.vert",
        "TerrainGen/assets/shaders/model/model.frag");

    s_Data.ModelShader->Bind();
    // glm::mat4 boxModel = glm::mat4(1.0f);
    // boxModel = glm::translate(boxModel, glm::vec3(0.0f, 2.0f, 0.0f));
    //   s_Data.ModelShader->SetMat4("uModel", boxModel);
    s_Data.ModelShader->SetVec3("uDirLight.direction", s_Data.SceneCompositionData.LightDirection);
    s_Data.ModelShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
    s_Data.ModelShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
    s_Data.ModelShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);

    //----------------- Random initial locations -------------------//
    // This info must come from the Poisson Disk algorithm
    for (int i = 0; i < s_Data.ModelPlacementData.Amount; i++)
    {
        float x = rand() % s_Data.Terrain->GetWidth();
        float y = rand() % s_Data.Terrain->GetHeight();
        s_Data.ModelPlacementData.Positions.emplace_back(x, y);
    }

    Enxus::Ref<Enxus::VertexBuffer> instanceBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(s_Data.ModelPlacementData.Amount * sizeof(glm::mat4));

    // Adding an empty instance buffer to hold the instance matrix
    Enxus::BufferLayout instanceLayout = {
        {Enxus::ShaderDataType::Mat4, "aInstanceMatrix"},
    };
    instanceBuffer->SetLayout(instanceLayout);
    // This must be done for each model..
    for (const auto &mesh : s_Data.Box->GetMeshes())
    {
        mesh->GetVertexArray()->AddVertexBuffer(instanceBuffer);
    }
}

void TerrainScene::InitSkyBox()
{
    //----------------- Sky Box -------------------//
    s_Data.SkyBox = Enxus::CreateRef<Enxus::SkyBox>();
    s_Data.SkyBox->SetCubeMapFaces(
        {"TerrainGen/assets/images/skybox/right.tga",
         "TerrainGen/assets/images/skybox/left.tga",
         "TerrainGen/assets/images/skybox/top.tga",
         "TerrainGen/assets/images/skybox/bottom.tga",
         "TerrainGen/assets/images/skybox/back.tga",
         "TerrainGen/assets/images/skybox/front.tga"});

    s_Data.SkyBoxShader = Enxus::CreateRef<Enxus::Shader>(
        "TerrainGen/assets/shaders/skybox/skybox.vert",
        "TerrainGen/assets/shaders/skybox/skybox.frag");
}

void TerrainScene::UpdateModelPositions()
{
    glm::mat4 initialMatrix = glm::mat4(1.0f);
    float scaleFactor = s_Data.Terrain->GetVertexDistance();
    float offsetX = (s_Data.Terrain->GetWidth() / 2) * scaleFactor;
    float offsetZ = (s_Data.Terrain->GetHeight() / 2) * scaleFactor;

    // translate to 0,0 (in the top left corner, no in the center)
    initialMatrix = glm::translate(initialMatrix, glm::vec3(-offsetX, 0.0f, -offsetZ));

    glm::mat4 *instanceMatrices = new glm::mat4[s_Data.ModelPlacementData.Amount];
    for (int i = 0; i < s_Data.ModelPlacementData.Amount; i++)
    {
        glm::vec2 coords = s_Data.ModelPlacementData.Positions[i];
        glm::vec3 vertexPos = s_Data.Terrain->GetVertexFromCoords(coords.x, coords.y);
        float offsetY = vertexPos.y;
        instanceMatrices[i] = glm::translate(initialMatrix, glm::vec3(coords.x * scaleFactor, offsetY, coords.y * scaleFactor));
        instanceMatrices[i] = glm::scale(instanceMatrices[i], glm::vec3(scaleFactor));
    }

    for (const auto &mesh : s_Data.Box->GetMeshes())
    {
        // Thas last added buffer is the instance buffer which was created in InitModels method
        auto &instanceBuffer = mesh->GetVertexArray()->GetVertexBuffers().back();
        instanceBuffer->SetData(instanceMatrices, s_Data.ModelPlacementData.Amount * sizeof(glm::mat4));
    }

    delete instanceMatrices;
}

void TerrainScene::OnUpdate()
{

    if (s_Data.SceneCompositionData.IsWireframe)
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE);
    else
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);

    //----------------- Draw Terrain -------------------//

    {
        s_Data.TerrainShader->Bind();
        s_Data.TerrainShader->SetMat4("uView", s_Data.CameraData.ViewMatrix);
        s_Data.TerrainShader->SetMat4("uProj", s_Data.CameraData.ProjectionMatrix);
        s_Data.TerrainShader->SetVec3("uCameraPos", s_Data.CameraData.Position);
        s_Data.TerrainShader->SetVec3("uDirLight.direction", s_Data.SceneCompositionData.LightDirection);

        s_Data.TerrainShader->SetFloat("uMinHeight", s_Data.Terrain->GetMinHeight());
        s_Data.TerrainShader->SetFloat("uMaxHeight", s_Data.Terrain->GetMaxHeight());
        s_Data.TerrainShader->SetInt("uNumOfColors", s_Data.TerrainBiomeData.NumOfBiomeLayers);

        for (int i = 0; i < s_Data.TerrainBiomeData.NumOfBiomeLayers; i++)
        {
            bool textureUsed = false;
            // Bind Textures
            if (s_Data.TerrainBiomeData.BiomeLayers[i].TextureIndex != 0)
            {
                s_Data.TerrainShader->SetFloat("uTexturesScale[" + std::to_string(i) + "]", s_Data.TerrainBiomeData.BiomeLayers[i].TextureScale);
                s_Data.TerrainShader->SetInt("uTerrainTextures[" + std::to_string(i) + "]", i);

                s_Data.TexturesList[s_Data.TerrainBiomeData.BiomeLayers[i].TextureIndex - 1]->Bind(i);

                textureUsed = true;
            }
            s_Data.TerrainShader->SetBool("uBiomeTextureUsed[" + std::to_string(i) + "]", textureUsed);

            s_Data.TerrainShader->SetFloat("uBiomeStartHeight[" + std::to_string(i) + "]", s_Data.TerrainBiomeData.BiomeLayers[i].StartHeight);
            s_Data.TerrainShader->SetFloat("uBiomeBlends[" + std::to_string(i) + "]", s_Data.TerrainBiomeData.BiomeLayers[i].BlendStrength);
            s_Data.TerrainShader->SetFloat("uBiomeColorStrength[" + std::to_string(i) + "]", s_Data.TerrainBiomeData.BiomeLayers[i].ColorStrength);
            s_Data.TerrainShader->SetVec3("uBiomeColors[" + std::to_string(i) + "]", s_Data.TerrainBiomeData.BiomeLayers[i].Color);
        }
        // pass the index of the textures used

        s_Data.Terrain->GetVertexArray()->Bind();

        uint32_t simplifiedWidth = s_Data.Terrain->GetSimplifiedWidth();
        uint32_t simplifiedHeight = s_Data.Terrain->GetSimplifiedHeight();

        const uint32_t numOfStrips = simplifiedHeight - 1;
        const uint32_t numOfVertPerStrip = simplifiedWidth * 2;
        for (unsigned int strip = 0; strip < numOfStrips; strip++)
        {
            size_t stripOffset = strip * numOfVertPerStrip * sizeof(unsigned int);
            GLCall(glDrawElements(GL_TRIANGLE_STRIP, numOfVertPerStrip, GL_UNSIGNED_INT, (void *)stripOffset));
        }
    }

    //----------------- Draw the models using instancing -------------------//
    {
        s_Data.ModelShader->Bind();
        s_Data.ModelShader->SetMat4("uView", s_Data.CameraData.ViewMatrix);
        s_Data.ModelShader->SetMat4("uProj", s_Data.CameraData.ProjectionMatrix);
        s_Data.ModelShader->SetVec3("uCameraPos", s_Data.CameraData.Position);
        s_Data.ModelShader->SetVec3("uDirLight.direction", s_Data.SceneCompositionData.LightDirection);

        const auto &meshes = s_Data.Box->GetMeshes();
        for (const auto &mesh : meshes)
        {
            const auto &meshTextures = mesh->GetTextures();
            for (uint8_t i = 0; i < meshTextures.size(); i++)
            {
                const Enxus::Ref<Enxus::TextureMesh2D> &texture = meshTextures[i];
                std::string textureName;
                switch (texture->GetType())
                {
                case Enxus::TextureType::DIFFUSE:
                    textureName = "diffuseMap";
                    break;
                case Enxus::TextureType::SPECULAR:
                    textureName = "specularMap";
                    break;
                // add normal map... etc
                default:
                    break;
                }
                s_Data.ModelShader->SetInt(("uMaterial." + textureName).c_str(), i);
                texture->Bind(i);
            }
            mesh->GetVertexArray()->Bind();
            // GLCall(glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
            GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, s_Data.ModelPlacementData.Amount));
        }
    }

    // Draw Skybox at last
    {
        // always render the skybox with fill mode and not wireframe
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);
        GLCall(glDepthMask(GL_FALSE));
        GLCall(glDepthFunc(GL_LEQUAL));

        glm::mat4 viewMatrix = glm::mat4(glm::mat3(s_Data.CameraData.ViewMatrix));

        s_Data.SkyBoxShader->Bind();
        s_Data.SkyBoxShader->SetMat4("uView", viewMatrix);
        s_Data.SkyBoxShader->SetMat4("uProj", s_Data.CameraData.ProjectionMatrix);

        s_Data.SkyBoxShader->SetInt("uSkyBoxTexture", 0);
        s_Data.SkyBox->Bind();

        s_Data.SkyBox->GetVertexArray()->Bind();

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

        GLCall(glDepthMask(GL_TRUE)); // Restoring state
        GLCall(glDepthFunc(GL_LESS)); // Restoring state
    }
}

void TerrainScene::UpdateTerrainNoiseMap(const std::vector<float> &noiseMap)
{
    s_Data.Terrain->SetNoiseMap(noiseMap);
    UpdateModelPositions();
}

void TerrainScene::UpdateTerrainDimensions(const TerrainDimensionPanelProps &props)
{
    // update the terrain according to the new panel props...
    if ((uint32_t)props.Width != s_Data.Terrain->GetWidth())
    {
        s_Data.Terrain->SetWidth(props.Width);
    }
    if ((uint32_t)props.Height != s_Data.Terrain->GetHeight())
    {
        s_Data.Terrain->SetHeight(props.Height);
    }
    if (props.LevelOfDetail != s_Data.Terrain->GetLevelOfDetail())
    {
        s_Data.Terrain->SetLevelOfDetail(props.LevelOfDetail);
    }
    if (props.Elevation != s_Data.Terrain->GetElevation())
    {
        s_Data.Terrain->SetElevation(props.Elevation);
        UpdateModelPositions();
    }
    if (props.VertexScale != s_Data.Terrain->GetVertexDistance())
    {
        s_Data.Terrain->SetVertexDistance(props.VertexScale);
        UpdateModelPositions();
    }
    if (props.ElevationCurve != (int)s_Data.Terrain->GetHeightElevationCurve())
    {
        s_Data.Terrain->SetHeightElevationCurve((AnimationCurve)props.ElevationCurve);
        UpdateModelPositions();
    }
}

void TerrainScene::UpdateTerrainBiome(const TerrainBiomePanelProps &props)
{
    s_Data.TerrainBiomeData = props;
}

void TerrainScene::UpdateSceneComposition(const SceneCompositionPanelProps &props)
{
    s_Data.SceneCompositionData = props;
}
