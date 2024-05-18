#ifndef TERRAIN_DIMENSION_PANEL_H
#define TERRAIN_DIMENSION_PANEL_H

struct TerrainDimensionPanelProps
{
    float VertexScale;
    float Elevation;
    int Width;
    int Height;
    int ElevationCurve;
    int LevelOfDetail;
};

class TerrainDimensionPanel
{
public:
    static void Init();
    static void ShutDown();

    static const TerrainDimensionPanelProps &GetPanelProps();

    static void OnImGuiRender();

    // TerrainDimensionPanelProps m_Props;
};

#endif