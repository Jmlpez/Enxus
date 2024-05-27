#include "ModelPlacementPanel.h"
#include "imgui/imgui.h"

static ModelPlacementPanelProps s_Props;

void ModelPlacementPanel::Init()
{
    s_Props.NumOfModels = 0;
    for (auto &modelData : s_Props.ModelsData)
    {
        modelData.Radius = 8.0f;
        modelData.Scale = 0.1f;
        modelData.OffsetHeight = 0.0f;
        modelData.Amount = 250;
        modelData.ModelIndex = 0;
        modelData.HeightRangeBegin = 0.0f;
        modelData.HeightRangeEnd = 1.0f;
    }
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
    static const char *enumModelArray[] = {
        "None",
        "Box",
        "BirchTree",
        "DeadTree",
        "MapleTree",
        "NormalTree",
        "PalmTree",
        "PineTree",
    };

    if (ImGui::BeginTabItem("Objects"))
    {
        ImGui::SliderInt("Models", &s_Props.NumOfModels, 0, 8);
        for (int i = 0; i < s_Props.NumOfModels; i++)
        {
            const std::string &layer = "Layer " + std::to_string(i + 1);
            ImGui::SeparatorText(layer.c_str());
            ImGui::PushID(layer.c_str());
            {
                ImGui::SliderFloat("Radius", &s_Props.ModelsData[i].Radius, 2.0f, 25.0f);
                ImGui::SliderInt("Amount", &s_Props.ModelsData[i].Amount, 1, 1000);
                ImGui::DragFloat("Scale", &s_Props.ModelsData[i].Scale, 0.001f);
                ImGui::DragFloat("OffsetY", &s_Props.ModelsData[i].OffsetHeight, 0.01f);
                ImGui::DragFloatRange2("Height Range", &s_Props.ModelsData[i].HeightRangeBegin, &s_Props.ModelsData[i].HeightRangeEnd, 0.001f, 0.0f, 1.0f, "Min: %.3f", "Max: %.3f", ImGuiSliderFlags_AlwaysClamp);
                ImGui::Combo("Model", &s_Props.ModelsData[i].ModelIndex, enumModelArray, IM_ARRAYSIZE(enumModelArray));
            }
            ImGui::PopID();
        }
        ImGui::EndTabItem();
    }
}
