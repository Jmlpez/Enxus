#ifndef TERRAIN_SCENE_H
#define TERRAIN_SCENE_H

#include "Enxus.h"
#include "TerrainDimensionPanel.h"
#include "TerrainBiomePanel.h"
#include "SceneCompositionPanel.h"
#include "TerrainMesh.h"

class TerrainScene
{
public:
    static void Init();
    static void ShutDown();
    static void OnUpdate();

    static void UpdateTerrainNoiseMap(const std::vector<float> &noiseMap);

    static void UpdateTerrainDimensions(const TerrainDimensionPanelProps &props);
    static void UpdateTerrainBiome(const TerrainBiomePanelProps &props);
    static void UpdateSceneComposition(const SceneCompositionPanelProps &props);

    static void SubmitCamera(const Enxus::Camera &camera);
};

#endif // TerrainScene.h