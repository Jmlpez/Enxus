#include "ModelPlacementPanel.h"
#include "imgui/imgui.h"

static ModelPlacementPanelProps s_Props;

void ModelPlacementPanel::Init()
{
    s_Props.Radius = 25.0f;
    s_Props.Amount = 1000;
}

void ModelPlacementPanel::ShutDown()
{
}

const ModelPlacementPanelProps &ModelPlacementPanel::GetPanelProps()
{
    return s_Props;
}

void ModelPlacementPanel::OnImGuiRender()
{
    if (ImGui::BeginTabItem("Objects"))
    {
        ImGui::SliderFloat("Radius", &s_Props.Radius, 5.0f, 100.0f);
        ImGui::DragFloat("Scale", &s_Props.Scale, 0.001f);
        ImGui::SliderInt("Amount", (int *)&s_Props.Amount, 1, 1000);
        ImGui::EndTabItem();
    }
}
