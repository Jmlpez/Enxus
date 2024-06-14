#include "TerrainScene.h"
#include "PoissonDiscSampler.h"
#include "ResourceManager.h"

struct TerrainSceneData
{
    Enxus::Ref<Enxus::Model> Box;
    Enxus::Ref<TerrainMesh> Terrain;

    struct ModelPositionData
    {
        Enxus::Scope<PoissonDiscSampler> RandomSampler;
        std::vector<glm::vec2> Positions;
        std::vector<glm::mat4> InstanceMatrix;
    };
    std::array<ModelPositionData, 8> ModelPositions;

    SceneCompositionPanelProps SceneCompositionData;
    ModelPlacementPanelProps ModelPlacementData;
    TerrainTexturePanelProps TerrainBiomeData;

    // Shaders
    Enxus::Ref<Enxus::Shader> TerrainShader;
    Enxus::Ref<Enxus::Shader> ModelShader;

    struct CameraData
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewProjectionMatrix;
        glm::vec3 Position;
    } CameraData;

    // Shadow Mapping
    Enxus::Ref<Enxus::ShadowMapFBO> ShadowMapFramebuffer;
    Enxus::Ref<Enxus::Shader> ShadowMapShader;
    glm::mat4 LightSpaceMatrix, LightOrthoProjectionMatrix, LightViewMatrix;

    // float FarPlane, NearPlane;
};

static TerrainSceneData s_Data;

void TerrainScene::SubmitCamera(const Enxus::Camera &camera)
{
    s_Data.CameraData.ViewMatrix = camera.GetViewMatrix();
    s_Data.CameraData.ProjectionMatrix = camera.GetProjectionMatrix();
    s_Data.CameraData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    s_Data.CameraData.Position = camera.GetPos();
}

Enxus::Ref<Enxus::ShadowMapFBO> TerrainScene::GetShadowFramebuffer()
{
    return s_Data.ShadowMapFramebuffer;
}

void TerrainScene::Init()
{

    //----------------- BLENDING -------------------//
    GLCall(glEnable(GL_BLEND));
    // Blending equation: source_color * source_factor + destination_color * destination_factor
    // usually source factor is the alpha channel of the source_color
    // and destination factor 1 - the alpha channel of the source_color
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //----------------- FACE CULLING -------------------//
    GLCall(glCullFace(GL_FRONT));
    GLCall(glFrontFace(GL_CW));

    // [TODO] Use Uniform Buffer Objects for things like camera and lighting
    InitTerrain();
    InitModels();
    InitShadowMapping();
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
    s_Data.TerrainShader->SetVec3("uDirLight.position", s_Data.SceneCompositionData.LightPosition);
    s_Data.TerrainShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
    s_Data.TerrainShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
    s_Data.TerrainShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);
}

void TerrainScene::InitModels()
{
    // For now the same shader will draw all the models
    s_Data.ModelShader = Enxus::CreateRef<Enxus::Shader>(
        "TerrainGen/assets/shaders/model/model.vert",
        "TerrainGen/assets/shaders/model/model.frag");

    s_Data.ModelShader->Bind();
    s_Data.ModelShader->SetVec3("uDirLight.position", s_Data.SceneCompositionData.LightPosition);
    s_Data.ModelShader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
    s_Data.ModelShader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
    s_Data.ModelShader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);

    //----------------- Random initial locations -------------------//
    // This info must come from the Poisson Disk algorithm
    for (int i = 0; i < s_Data.ModelPlacementData.MaxNumOfModels; i++)
    {
        const float initialRadius = 10.0f;
        const uint32_t initialAmount = 100;
        s_Data.ModelPositions[i].RandomSampler =
            Enxus::CreateScope<PoissonDiscSampler>(s_Data.Terrain->GetWidth() - 1,
                                                   s_Data.Terrain->GetHeight() - 1,
                                                   initialRadius,
                                                   initialAmount);
        // Initially there are no models

        s_Data.ModelPositions[i].RandomSampler->CalculatePoints();
        s_Data.ModelPositions[i].Positions = s_Data.ModelPositions[i].RandomSampler->GetSampledPoints();
    }

    for (auto &model : ResourceManager::GetModelsList())
    {
        Enxus::Ref<Enxus::VertexBuffer>
            instanceBuffer = Enxus::CreateRef<Enxus::VertexBuffer>(s_Data.ModelPlacementData.MaxAmount * sizeof(glm::mat4));

        // Adding an empty instance buffer to hold the instance matrix
        Enxus::BufferLayout instanceLayout = {
            {Enxus::ShaderDataType::Mat4, "aInstanceMatrix"},
        };
        instanceBuffer->SetLayout(instanceLayout);
        // This must be done for each model..
        for (const auto &mesh : model->GetMeshes())
        {
            mesh->GetVertexArray()->AddVertexBuffer(instanceBuffer);
        }
    }
}

void TerrainScene::InitShadowMapping()
{
    Enxus::ShadowMapSpecification spec(2048, 2048);
    s_Data.ShadowMapFramebuffer = Enxus::CreateRef<Enxus::ShadowMapFBO>(spec);
    s_Data.ShadowMapShader = Enxus::CreateRef<Enxus::Shader>(
        ResourceManager::GetResourcesPath().Shaders + "shadow-map/shadow-map.vert",
        ResourceManager::GetResourcesPath().Shaders + "shadow-map/shadow-map.frag");

    // float nearPlane = 0.0f, farPlane = 25.0f;
    s_Data.LightOrthoProjectionMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.1f, 45.0f);

    s_Data.LightViewMatrix = glm::lookAt(s_Data.SceneCompositionData.LightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    s_Data.LightSpaceMatrix = s_Data.LightOrthoProjectionMatrix * s_Data.LightViewMatrix;
}

void TerrainScene::UpdateModelPositions()
{

    glm::mat4 initialMatrix = glm::mat4(1.0f);
    float scaleFactor = s_Data.Terrain->GetVertexDistance();
    float offsetX = (s_Data.Terrain->GetWidth() / 2) * scaleFactor;
    float offsetZ = (s_Data.Terrain->GetHeight() / 2) * scaleFactor;

    // translate to 0,0 (in the top left corner, no in the center)
    initialMatrix = glm::translate(initialMatrix, glm::vec3(-offsetX, 0.0f, -offsetZ));

    for (int i = 0; i < s_Data.ModelPlacementData.NumOfModels; i++)
    {
        const auto &currentModelData = s_Data.ModelPlacementData.ModelsData[i];
        const auto &currentModelPositions = s_Data.ModelPositions[i];

        // glm::mat4 *instanceMatrices = new glm::mat4[currentModelData.Amount];
        std::vector<glm::mat4> matrices;
        for (uint32_t j = 0; j < (uint32_t)currentModelData.Amount && j < currentModelPositions.Positions.size(); j++)
        {
            glm::vec2 coords = currentModelPositions.Positions[j];
            glm::vec3 vertexPos = s_Data.Terrain->GetVertexFromCoords(coords.x, coords.y);
            float offsetY = vertexPos.y;

            float minHeight = s_Data.Terrain->GetMinHeight();
            float maxHeight = s_Data.Terrain->GetMaxHeight();
            float heightPercent = std::clamp((offsetY - minHeight) / (maxHeight - minHeight), 0.0f, 1.0f);

            // If its outside of the current boundaries of the panel
            if (heightPercent < currentModelData.HeightRangeBegin || heightPercent > currentModelData.HeightRangeEnd)
            {
                continue;
            }

            glm::mat4 modelMatrix = glm::translate(initialMatrix, glm::vec3(coords.x * scaleFactor, offsetY, coords.y * scaleFactor));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(currentModelData.Scale));
            matrices.emplace_back(modelMatrix);
        }
        s_Data.ModelPositions[i].InstanceMatrix = std::move(matrices);
    }
}

void TerrainScene::OnShadowPass()
{
    // Avoid shadows calculation in case the checkbox is unset in the panel
    if (!s_Data.SceneCompositionData.UseShadows)
    {
        return;
    }
    // implement shadow mapping
    s_Data.ShadowMapFramebuffer->Bind();
    {
        s_Data.ShadowMapShader->Bind();
        s_Data.ShadowMapShader->SetMat4("uLightSpaceMatrix", s_Data.LightSpaceMatrix);
        {
            // Draw Terrain

            s_Data.ShadowMapShader->SetBool("uUseInstancing", 0);
            s_Data.Terrain->Draw();
        }
        // Draw Models
        {
            s_Data.ShadowMapShader->SetBool("uUseInstancing", 1);
            const auto &modelsList = ResourceManager::GetModelsList();
            for (int i = 0; i < s_Data.ModelPlacementData.NumOfModels; i++)
            {
                const uint32_t modelIndex = s_Data.ModelPlacementData.ModelsData[i].ModelIndex;
                // If no model are selected, ignore it
                // The 0 index is reserverd for the NoneModel value
                if (modelIndex == 0)
                    continue;

                const auto &model = modelsList[modelIndex - 1];
                for (auto &mesh : model->GetMeshes())
                {
                    s_Data.ShadowMapShader->SetFloat("uOffsetY", s_Data.ModelPlacementData.ModelsData[i].OffsetHeight);
                    mesh->GetVertexArray()->Bind();
                    const uint32_t renderedAmount = s_Data.ModelPositions[i].InstanceMatrix.size();

                    // Thas last added buffer is the instance buffer which was created in InitModels method
                    auto &instanceBuffer = mesh->GetVertexArray()->GetVertexBuffers().back();
                    instanceBuffer->SetData(&s_Data.ModelPositions[i].InstanceMatrix[0], renderedAmount * sizeof(glm::mat4));
                    GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, renderedAmount));
                }
            }
        }
    }
    // The skybox is not needed
    s_Data.ShadowMapFramebuffer->Unbind();
}

void TerrainScene::OnRenderPass()
{

    if (s_Data.SceneCompositionData.IsWireframe)
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::LINE);
    else
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);

    //----------------- Draw Terrain -------------------//
    {
        s_Data.TerrainShader->Bind();
        s_Data.TerrainShader->SetMat4("uViewProj", s_Data.CameraData.ViewProjectionMatrix);
        s_Data.TerrainShader->SetVec3("uCameraPos", s_Data.CameraData.Position);
        s_Data.TerrainShader->SetVec3("uDirLight.position", s_Data.SceneCompositionData.LightPosition);

        s_Data.TerrainShader->SetFloat("uMinHeight", s_Data.Terrain->GetMinHeight());
        s_Data.TerrainShader->SetFloat("uMaxHeight", s_Data.Terrain->GetMaxHeight());
        s_Data.TerrainShader->SetInt("uNumOfLayers", s_Data.TerrainBiomeData.NumOfTextureLayers);

        s_Data.TerrainShader->SetMat4("uLightSpaceMatrix", s_Data.LightSpaceMatrix);

        // Bind the shadow map
        s_Data.TerrainShader->SetBool("uUseShadows", s_Data.SceneCompositionData.UseShadows);
        s_Data.TerrainShader->SetInt("uShadowMap", 0);
        s_Data.ShadowMapFramebuffer->BindShadowTexture(0);

        const auto &terrainTextureList = ResourceManager::GetTexturesList();
        for (int i = 0, textureSlot = 1; i < s_Data.TerrainBiomeData.NumOfTextureLayers; i++, textureSlot++)
        {
            bool textureUsed = false;
            // Bind Textures
            std::string index = std::to_string(i);
            if (s_Data.TerrainBiomeData.TextureLayers[i].TextureIndex != 0)
            {
                s_Data.TerrainShader->SetFloat("uTexturesScale[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].TextureScale);
                s_Data.TerrainShader->SetInt("uTerrainTextures[" + index + "]", textureSlot);

                terrainTextureList[s_Data.TerrainBiomeData.TextureLayers[i].TextureIndex - 1]->Bind(textureSlot);

                textureUsed = true;
            }
            s_Data.TerrainShader->SetBool("uTextureUsed[" + index + "]", textureUsed);
            s_Data.TerrainShader->SetFloat("uBlendBoundaries[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].BlendBoundaries);
            s_Data.TerrainShader->SetFloat("uColorStrength[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].ColorStrength);
            s_Data.TerrainShader->SetVec3("uColors[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].Color);
            s_Data.TerrainShader->SetFloat("uSlopeHeightBegin[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].SlopeHeightBegin);
            s_Data.TerrainShader->SetFloat("uSlopeHeightEnd[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].SlopeHeightEnd);
            s_Data.TerrainShader->SetFloat("uSlopeThreshold[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].SlopeThreshold);
            s_Data.TerrainShader->SetFloat("uBlendSlope[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].BlendSlope);
            s_Data.TerrainShader->SetFloat("uBlendLayer[" + index + "]", s_Data.TerrainBiomeData.TextureLayers[i].BlendLayer);
        }
        s_Data.Terrain->Draw();
    }

    //----------------- Draw the models using instancing -------------------//
    {
        GLCall(glEnable(GL_CULL_FACE));

        s_Data.ModelShader->Bind();
        s_Data.ModelShader->SetMat4("uViewProj", s_Data.CameraData.ViewProjectionMatrix);
        s_Data.ModelShader->SetVec3("uCameraPos", s_Data.CameraData.Position);
        s_Data.ModelShader->SetVec3("uDirLight.position", s_Data.SceneCompositionData.LightPosition);

        const auto &modelsList = ResourceManager::GetModelsList();
        for (int i = 0; i < s_Data.ModelPlacementData.NumOfModels; i++)
        {
            const uint32_t modelIndex = s_Data.ModelPlacementData.ModelsData[i].ModelIndex;

            // If no model are selected, ignore it
            // The 0 index is reserverd for the NoneModel value
            if (modelIndex == 0)
                continue;

            const auto &model = modelsList[modelIndex - 1];
            for (auto &mesh : model->GetMeshes())
            {
                const auto &meshTextures = mesh->GetTextures();
                for (uint8_t i = 0, textureSlot = 0; i < meshTextures.size(); i++, textureSlot++)
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
                    s_Data.ModelShader->SetInt(("uMaterial." + textureName).c_str(), textureSlot);
                    texture->Bind(textureSlot);
                }

                s_Data.ModelShader->SetFloat("uOffsetY", s_Data.ModelPlacementData.ModelsData[i].OffsetHeight);

                mesh->GetVertexArray()->Bind();
                const uint32_t renderedAmount = s_Data.ModelPositions[i].InstanceMatrix.size();

                // Thas last added buffer is the instance buffer which was created in InitModels method
                auto &instanceBuffer = mesh->GetVertexArray()->GetVertexBuffers().back();
                instanceBuffer->SetData(&s_Data.ModelPositions[i].InstanceMatrix[0], renderedAmount * sizeof(glm::mat4));
                GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, renderedAmount));
            }
        }
        GLCall(glDisable(GL_CULL_FACE));
    }

    // Draw Skybox at last
    {
        // always render the skybox with fill mode and not wireframe
        Enxus::Renderer::SetPolygonMode(Enxus::PolygonMode::FILL);
        GLCall(glDepthMask(GL_FALSE));
        GLCall(glDepthFunc(GL_LEQUAL));

        glm::mat4 viewMatrix = glm::mat4(glm::mat3(s_Data.CameraData.ViewMatrix));

        const auto &skybox = ResourceManager::GetSkyBox();
        const auto &skyboxShader = ResourceManager::GetSkyBoxShader();

        skyboxShader->Bind();
        skyboxShader->SetMat4("uView", viewMatrix);
        skyboxShader->SetMat4("uProj", s_Data.CameraData.ProjectionMatrix);

        skyboxShader->SetInt("uSkyBoxTexture", 0);

        skybox->Bind();
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

void TerrainScene::UpdateTerrainDimensions(const TerrainMeshPanelProps &props)
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
    if (props.HeightCurve != s_Data.Terrain->GetHeightCurve())
    {
        s_Data.Terrain->SetHeightCurve(props.HeightCurve);
        UpdateModelPositions();
    }
}

void TerrainScene::UpdateTerrainBiome(const TerrainTexturePanelProps &props)
{
    s_Data.TerrainBiomeData = props;
}

void TerrainScene::UpdateSceneComposition(const SceneCompositionPanelProps &props)
{

    //----------------- DEBUGGING PURPOSES -------------------//
    if (props.OrthoProj.IsDebugging)
    {
        s_Data.LightOrthoProjectionMatrix = glm::ortho(-props.OrthoProj.WidthLimit,
                                                       props.OrthoProj.WidthLimit,
                                                       -props.OrthoProj.HeightLimit,
                                                       props.OrthoProj.HeightLimit,
                                                       props.OrthoProj.NearPlane,
                                                       props.OrthoProj.FarPlane);

        s_Data.LightSpaceMatrix = s_Data.LightOrthoProjectionMatrix * s_Data.LightViewMatrix;
    }

    if (s_Data.SceneCompositionData.LightPosition != props.LightPosition)
    {
        s_Data.LightViewMatrix = glm::lookAt(props.LightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        s_Data.LightSpaceMatrix = s_Data.LightOrthoProjectionMatrix * s_Data.LightViewMatrix;
    }

    s_Data.SceneCompositionData = props;
}

void TerrainScene::UpdateModelPlacement(const ModelPlacementPanelProps &props)
{

    if (s_Data.ModelPlacementData.NumOfModels != props.NumOfModels)
    {
        s_Data.ModelPlacementData.NumOfModels = props.NumOfModels;
    }

    // Update the model location properties
    for (int i = 0; i < props.NumOfModels; i++)
    {
        auto &currentModelData = s_Data.ModelPlacementData.ModelsData[i];
        auto &propsModelData = props.ModelsData[i];
        if (currentModelData.RandomGeneratorSeed != propsModelData.RandomGeneratorSeed)
        {
            currentModelData.RandomGeneratorSeed = propsModelData.RandomGeneratorSeed;
            s_Data.ModelPositions[i].RandomSampler->SetSeed(propsModelData.RandomGeneratorSeed);
            s_Data.ModelPositions[i].Positions = s_Data.ModelPositions[i].RandomSampler->GetSampledPoints();
            UpdateModelPositions();
        }
        if (currentModelData.Radius != propsModelData.Radius)
        {
            currentModelData.Radius = propsModelData.Radius;
            s_Data.ModelPositions[i].RandomSampler->SetRadius(propsModelData.Radius);
            s_Data.ModelPositions[i].Positions = s_Data.ModelPositions[i].RandomSampler->GetSampledPoints();
            UpdateModelPositions();
        }
        if (currentModelData.Scale != propsModelData.Scale)
        {
            currentModelData.Scale = propsModelData.Scale;
            UpdateModelPositions();
        }
        if (currentModelData.Amount != propsModelData.Amount)
        {
            currentModelData.Amount = propsModelData.Amount;
            s_Data.ModelPositions[i].RandomSampler->SetAmount(propsModelData.Amount);
            s_Data.ModelPositions[i].Positions = s_Data.ModelPositions[i].RandomSampler->GetSampledPoints();
            UpdateModelPositions();
        }
        if (currentModelData.ModelIndex != propsModelData.ModelIndex)
        {
            currentModelData.ModelIndex = propsModelData.ModelIndex;
        }
        if (currentModelData.OffsetHeight != propsModelData.OffsetHeight)
        {
            currentModelData.OffsetHeight = propsModelData.OffsetHeight;
        }
        if (currentModelData.HeightRangeBegin != propsModelData.HeightRangeBegin)
        {
            currentModelData.HeightRangeBegin = propsModelData.HeightRangeBegin;
            s_Data.ModelPositions[i].Positions = s_Data.ModelPositions[i].RandomSampler->GetSampledPoints();
            UpdateModelPositions();
        }
        if (currentModelData.HeightRangeEnd != propsModelData.HeightRangeEnd)
        {
            currentModelData.HeightRangeEnd = propsModelData.HeightRangeEnd;
            s_Data.ModelPositions[i].Positions = s_Data.ModelPositions[i].RandomSampler->GetSampledPoints();
            UpdateModelPositions();
        }
    }
}
