#include "TerrainDimensionPanel.h"
#include "TerrainMesh.h"
#include "imgui/imgui.h"

static TerrainDimensionPanelProps s_Props;

void TerrainDimensionPanel::Init()
{
    s_Props.VertexScale = 0.065f;
    s_Props.Elevation = 1.5f;
    s_Props.Width = 241;
    s_Props.Height = 241;

    // AnimationCurve::Linear = 0
    s_Props.ElevationCurve = 0;
    s_Props.LevelOfDetail = 0;
}

void TerrainDimensionPanel::ShutDown()
{
}

const TerrainDimensionPanelProps &TerrainDimensionPanel::GetPanelProps()
{
    return s_Props;
}

void TerrainDimensionPanel::OnImGuiRender()
{
    static const char *enumTerrainAnimationCurve[] = {"Linear", "EaseInQuad", "EaseInCubic", "EaseInQuart", "EaseInQuint"};

    if (ImGui::BeginTabItem("Terrain Dimensions"))
    {
        ImGui::SliderInt("Level Of Detail", &s_Props.LevelOfDetail, 0, 6);
        ImGui::DragFloat("Elevation", &s_Props.Elevation, 0.01f, 0.0f);
        ImGui::DragFloat("Distance", &s_Props.VertexScale, 0.001f, 0.01f, 2.0f);
        ImGui::SliderInt("Terrain Width", &s_Props.Width, 50, 500);
        ImGui::SliderInt("Terrain Height", &s_Props.Height, 50, 500);

        ImGui::Combo("Elevation Curve", &s_Props.ElevationCurve, enumTerrainAnimationCurve, IM_ARRAYSIZE(enumTerrainAnimationCurve));
        ImGui::EndTabItem();
    }
}