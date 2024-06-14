#ifndef TERRAIN_MESH_PANEL_H
#define TERRAIN_MESH_PANEL_H

#include "Enxus.h"

struct TerrainMeshPanelProps
{
    float VertexScale;
    float Elevation;
    int Width;
    int Height;
    int LevelOfDetail;
    Enxus::AnimationCurveEditor HeightCurve;
};

class TerrainMeshPanel
{
public:
    static void Init();
    static void ShutDown();

    static const TerrainMeshPanelProps &GetPanelProps();

    static void OnImGuiRender();
};

#endif