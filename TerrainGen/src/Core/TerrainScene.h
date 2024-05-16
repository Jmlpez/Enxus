#ifndef TERRAIN_SCENE_H
#define TERRAIN_SCENE_H

#include "Enxus.h"
#include "TerrainDimensionPanel.h"
#include "SceneCompositionPanel.h"
#include "TerrainMesh.h"

class TerrainScene
{
public:
    TerrainScene();
    ~TerrainScene();

    void Init();
    void OnUpdate();

    void UpdateTerrainNoiseMap(const std::vector<float> noiseMap);
    void UpdateTerrainDimensions(TerrainDimensionPanelProps &props);
    void UpdateSceneComposition(const SceneCompositionPanelProps &props);
    void SubmitCamera(const Enxus::Camera &camera);

private:
    Enxus::Scope<TerrainMesh> m_TerrainMesh;

    SceneCompositionPanelProps m_SceneCompositionData;

    // SkyBox
    Enxus::Ref<Enxus::SkyBox> m_SkyBox;
    Enxus::Ref<Enxus::Shader> m_SkyBoxShader;

    // Shaders
    Enxus::Ref<Enxus::Shader> m_TerrainShader;

    struct CameraData
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::vec3 Position;
    } m_CameraData;
};

#endif // TerrainScene.h