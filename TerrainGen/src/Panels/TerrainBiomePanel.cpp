#include "TerrainBiomePanel.h"
#include "ResourceManager.h"
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
    static const char *enumTerrainTexturesArray[] = {"None", "Water", "Grass", "Rocks1", "Rocks2", "Sandy Grass", "Stony Ground", "Snow"};
    static const int arrSize = IM_ARRAYSIZE(enumTerrainTexturesArray);

    const auto &texturesList = ResourceManager::GetTexturesList();

    if (ImGui::BeginTabItem("Biome"))
    {
        ImGui::SliderInt("Layers", &s_Props.NumOfBiomeLayers, 0, 8);

        for (int i = 0; i < s_Props.NumOfBiomeLayers; i++)
        {
            std::string index = std::to_string(i + 1);
            std::string uiName = "Layer " + index;
            ImGui::PushID(uiName.c_str());
            ImGui::SeparatorText(uiName.c_str());

            {
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

                const std::string dragDropPayloadName = "Terrain Texture Index";
                ImGui::BeginGroup();
                if (s_Props.BiomeLayers[i].TextureIndex == 0)
                {
                    ImGui::Button(enumTerrainTexturesArray[s_Props.BiomeLayers[i].TextureIndex], ImVec2(80, 80));
                }
                else
                {
                    const intptr_t textureId = texturesList[s_Props.BiomeLayers[i].TextureIndex - 1]->GetRendererId();
                    ImGui::Image((void *)textureId, ImVec2(80, 80));
                }
                ImGui::EndGroup();

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(dragDropPayloadName.c_str()))
                    {
                        int payloadIndex = *(const int *)payload->Data;
                        s_Props.BiomeLayers[i].TextureIndex = payloadIndex;
                        ImGui::EndDragDropTarget();
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("X", ImVec2(20, 20)))
                {
                    s_Props.BiomeLayers[i].TextureIndex = 0;
                }
            }

            ImGui::PopID();
        }

        ImGui::EndTabItem();
    }

    {
        ImGui::Begin("Terrain Textures");

        const float itemHorizontalSpacing = 15;
        ImGuiStyle &style = ImGui::GetStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(itemHorizontalSpacing, style.ItemSpacing.y));

        ImVec2 buttonSize(80, 80);
        float windowVisibleX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        for (int i = 1, count = 0; i < arrSize; i++, count++)
        {
            const Enxus::Ref<Enxus::TextureMesh2D> texture = texturesList[i - 1];

            const std::string dragDropPayloadName = "Terrain Texture Index";
            ImGui::PushID(i);

            {
                ImGui::BeginGroup();
                {
                    intptr_t textureId = texture->GetRendererId();
                    ImGui::Image((void *)textureId, buttonSize);
                    ImGui::Text(enumTerrainTexturesArray[i]);
                }
                ImGui::EndGroup();

                float lastButtonX = ImGui::GetItemRectMax().x;
                float nextButtonX = lastButtonX + itemHorizontalSpacing + buttonSize.x;
                if (count + 1 < arrSize && nextButtonX < windowVisibleX)
                    ImGui::SameLine();

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    ImGui::SetDragDropPayload(dragDropPayloadName.c_str(), &i, sizeof(int));
                    ImGui::Text(enumTerrainTexturesArray[i]);
                    ImGui::EndDragDropSource();
                }
            }

            ImGui::PopID();
        }
        ImGui::PopStyleVar();
        ImGui::End();
    }
}
