#ifndef TERRAIN_DIMENSION_PANEL_H
#define TERRAIN_DIMENSION_PANEL_H

#include "Enxus.h"

struct TerrainDimensionPanelProps
{
    float VertexScale;
    float Elevation;
    int Width;
    int Height;
    int ElevationCurve;
    int LevelOfDetail;
    Enxus::AnimationCurveEditor HeightCurve;
};

class TerrainDimensionPanel
{
public:
    static void Init();
    static void ShutDown();

    static const TerrainDimensionPanelProps &GetPanelProps();

    static void OnImGuiRender();
};

#endif