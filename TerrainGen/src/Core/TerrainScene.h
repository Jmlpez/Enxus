#ifndef TERRAIN_SCENE_H
#define TERRAIN_SCENE_H

#include "Enxus.h"
#include "TerrainDimensionPanel.h"
#include "TerrainBiomePanel.h"
#include "SceneCompositionPanel.h"
#include "ModelPlacementPanel.h"
#include "TerrainMesh.h"

class TerrainScene
{
public:
    static void Init();
    static void ShutDown();
    static void OnShadowPass();
    static void OnRenderPass();

    static void UpdateTerrainNoiseMap(const std::vector<float> &noiseMap);
    static void UpdateTerrainDimensions(const TerrainDimensionPanelProps &props);
    static void UpdateTerrainBiome(const TerrainBiomePanelProps &props);
    static void UpdateSceneComposition(const SceneCompositionPanelProps &props);
    static void UpdateModelPlacement(const ModelPlacementPanelProps &props);

    static void SubmitCamera(const Enxus::Camera &camera);

private:
    static void InitTerrain();
    static void InitModels();

    static void UpdateModelPositions();
};

#endif // TerrainScene.h