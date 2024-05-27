#ifndef TERRAIN_BIOME_PANEL_H
#define TERRAIN_BIOME_PANEL_H

#include "Enxus.h"

struct TerrainBiomePanelProps
{
    int NumOfBiomeLayers = 0;

    struct BiomeLayerData
    {
        glm::vec3 Color;
        float ColorStrength;
        float StartHeight;
        float BlendStrength;
        float TextureScale;
        // This the index in the full array of textures
        int TextureIndex;
    };

    // Add the 8 as a constant
    std::array<BiomeLayerData, 8> BiomeLayers;
};

class TerrainBiomePanel
{
public:
    static void Init();
    static void ShutDown();

    static const TerrainBiomePanelProps &GetPanelProps();

    static void OnImGuiRender();
};

#endif