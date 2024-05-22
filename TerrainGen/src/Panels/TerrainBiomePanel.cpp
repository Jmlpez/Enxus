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
        biomeData.BlendStrength = 0.15f;
        biomeData.TextureScale = 0.1f;
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

        ImGui::SliderInt("Layers", &s_Props.NumOfBiomeLayers, 0, 8);

        for (int i = 0; i < s_Props.NumOfBiomeLayers; i++)
        {
            std::string index = std::to_string(i + 1);
            std::string uiName = "Layer " + index;
            ImGui::PushID(uiName.c_str());
            ImGui::SeparatorText(uiName.c_str());

            float prevStartHeight = (i != 0) ? s_Props.BiomeLayers[i - 1].StartHeight : 0.0f;
            if (i != 0 && s_Props.BiomeLayers[i].StartHeight < prevStartHeight)
            {
                s_Props.BiomeLayers[i].StartHeight = prevStartHeight;
            }

            ImGui::ColorEdit3("Color", glm::value_ptr(s_Props.BiomeLayers[i].Color));
            ImGui::SliderFloat("Color Strength", &s_Props.BiomeLayers[i].ColorStrength, 0.0f, 1.0f);
            ImGui::SliderFloat("Start Height", &s_Props.BiomeLayers[i].StartHeight, prevStartHeight, 1.0f);
            ImGui::SliderFloat("Blend Strength", &s_Props.BiomeLayers[i].BlendStrength, 0.0f, 1.0f);
            ImGui::DragFloat("Texture Scale", &s_Props.BiomeLayers[i].TextureScale, 0.001f, 0.0f, 1.0f);
            ImGui::Combo("Texture", &s_Props.BiomeLayers[i].TextureIndex, enumTerrainTextures, IM_ARRAYSIZE(enumTerrainTextures));

            ImGui::PopID();
        }
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::EndTabItem();
    }
}