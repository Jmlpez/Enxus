#include "TerrainMeshPanel.h"
#include "TerrainMesh.h"
#include "imgui/imgui.h"

static TerrainMeshPanelProps s_Props;

void TerrainMeshPanel::Init()
{
    s_Props.VertexScale = 0.065f;
    s_Props.Elevation = 1.5f;
    s_Props.Width = 241;
    s_Props.Height = 241;
    s_Props.LevelOfDetail = 0;
}

void TerrainMeshPanel::ShutDown()
{
}

const TerrainMeshPanelProps &TerrainMeshPanel::GetPanelProps()
{
    return s_Props;
}

void TerrainMeshPanel::OnImGuiRender()
{
    static const char *sizeListId[] = {"241x241 - Small", "481x481 - Medium", "721x721 - Large"};
    static glm::vec2 sizeList[] = {{241, 241}, {481, 481}, {721, 721}};
    static int selectedSizeIndex = 0;

    static bool showAnimationCurveWindow = true;

    if (ImGui::BeginTabItem("Terrain Dimensions"))
    {
        ImGui::SliderInt("Level Of Detail", &s_Props.LevelOfDetail, 0, 6);
        ImGui::DragFloat("Elevation", &s_Props.Elevation, 0.01f, 0.0f);
        ImGui::DragFloat("Distance", &s_Props.VertexScale, 0.001f, 0.01f, 2.0f);

        if (ImGui::Combo("Size", &selectedSizeIndex, sizeListId, IM_ARRAYSIZE(sizeListId)))
        {
            s_Props.Width = sizeList[selectedSizeIndex].x;
            s_Props.Height = sizeList[selectedSizeIndex].y;
        }

        {
            // Animation Curve
            if (ImGui::Button("Animation Curve"))
            {
                showAnimationCurveWindow = !showAnimationCurveWindow;
            }

            if (showAnimationCurveWindow)
            {
                ImGui::Begin("Height Curve");
                s_Props.HeightCurve.OnImGuiRender();
                ImGui::End();
            }
        }
        ImGui::EndTabItem();
    }
}