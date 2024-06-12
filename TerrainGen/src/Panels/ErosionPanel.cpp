#include "ErosionPanel.h"
#include "imgui/imgui.h"

static ErosionPanelProps s_Props;

void ErosionPanel::Init()
{
    s_Props.IsErosionOn = false;
    s_Props.RevertErosion = false;
    s_Props.Iterations = 70000;
}

void ErosionPanel::ShutDown()
{
}

const ErosionPanelProps &ErosionPanel::GetPanelProps()
{
    return s_Props;
}

void ErosionPanel::OnImGuiRender()
{
    if (ImGui::Begin("Erosion"))
    {
        s_Props.RevertErosion = false;
        ImGui::Checkbox("Erosion ON", &s_Props.IsErosionOn);
        if (ImGui::Button("Revert Erosion"))
        {
            s_Props.RevertErosion = true;
        }
        ImGui::DragInt("Iterations", &s_Props.Iterations, 100, 1, 200000, "%d", ImGuiSliderFlags_AlwaysClamp);
        ImGui::End();
    }
}
