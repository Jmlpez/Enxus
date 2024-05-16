#include "TerrainBiomePanel.h"
#include "imgui/imgui.h"

static TerrainBiomePanelProps s_Props;

void TerrainBiomePanel::Init()
{
    s_Props.NumOfBiomeLayers = 0;
    for (auto &biomeData : s_Props.BiomeLayers)
    {
        biomeData.Color = glm::vec3(1.0f);
        biomeData.ColorStrength = 0.0f;
        biomeData.StartHeight = 0.0f;
        biomeData.BlendStrength = 0.5f;
        biomeData.TextureScale = 1.0f;
        biomeData.TextureIndex = 0;
    }
}

void TerrainBiomePanel::ShutDown()
{
}

const TerrainBiomePanelProps &TerrainBiomePanel::GetPanelProps()
{
    return s_Props;
}

void TerrainBiomePanel::OnImGuiRender()
{
    static const char *enumTerrainTextures[] = {"None", "Water", "Grass", "Rocks1", "Rocks2", "Sandy Grass", "Stony Ground", "Snow"};

    if (ImGui::BeginTabItem("Biome"))
    {

        ImGui::SliderInt("Layers", &s_Props.NumOfBiomeLayers, 1, 8);

        for (int i = 0; i < s_Props.NumOfBiomeLayers; i++)
        {
            std::string index = std::to_string(i);
            std::string uiName = "Layer " + index;
            ImGui::SeparatorText(uiName.c_str());
            ImGui::ColorEdit3(("Color - " + index).c_str(), glm::value_ptr(s_Props.BiomeLayers[i].Color));
            ImGui::SliderFloat(("Color Strength - " + index).c_str(), &s_Props.BiomeLayers[i].ColorStrength, 0.0f, 1.0f);
            ImGui::SliderFloat(("Start Height - " + index).c_str(), &s_Props.BiomeLayers[i].StartHeight, 0.0f, 1.0f);
            ImGui::SliderFloat(("Blend Strength - " + index).c_str(), &s_Props.BiomeLayers[i].BlendStrength, 0.0f, 1.0f);
            ImGui::DragFloat(("Texture Scale - " + index).c_str(), &s_Props.BiomeLayers[i].TextureScale, 0.1f);
            ImGui::Combo(("Texture - " + index).c_str(), &s_Props.BiomeLayers[i].TextureIndex, enumTerrainTextures, IM_ARRAYSIZE(enumTerrainTextures));
        }
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::EndTabItem();
    }
}