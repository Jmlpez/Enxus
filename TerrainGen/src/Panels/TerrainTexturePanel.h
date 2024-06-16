#ifndef TERRAIN_TEXTURE_PANEL_H
#define TERRAIN_TEXTURE_PANEL_H

#include "Enxus.h"

struct TerrainTexturePanelProps
{
    int NumOfTextureLayers = 0;

    struct TextureLayerData
    {
        glm::vec3 Color;
        float ColorStrength;
        float BlendBoundaries;
        float TextureScale;
        float BlendLayer;

        float SlopeHeightBegin;
        float SlopeHeightEnd;
        float SlopeThreshold;
        float BlendSlope;

        // This the index in the full array of textures
        int TextureIndex;
    };

    // Add the 8 as a constant
    std::array<TextureLayerData, 8> TextureLayers;
};

class TerrainTexturePanel
{
public:
    static void Init();
    static void ShutDown();

    static const TerrainTexturePanelProps &GetPanelProps();

    static void OnImGuiRender();
};

#endif