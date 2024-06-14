#include "TerrainTexturePanel.h"
#include "ResourceManager.h"
#include "imgui/imgui.h"

static TerrainTexturePanelProps s_Props;

void TerrainTexturePanel::Init()
{
    s_Props.NumOfTextureLayers = 0;
    for (auto &textureData : s_Props.TextureLayers)
    {
        textureData.Color = glm::vec3(1.0f);
        textureData.ColorStrength = 0.0f;
        textureData.BlendBoundaries = 0.15f;
        textureData.TextureScale = 0.1f;
        textureData.BlendLayer = 1.0f;
        textureData.TextureIndex = 0;

        textureData.SlopeHeightBegin = 0.0f;
        textureData.SlopeHeightEnd = 1.0f;
        textureData.SlopeThreshold = 0.2f;
        textureData.BlendSlope = 0.1f;
    }
}

void TerrainTexturePanel::ShutDown()
{
}

const TerrainTexturePanelProps &TerrainTexturePanel::GetPanelProps()
{
    return s_Props;
}

void TerrainTexturePanel::OnImGuiRender()
{
    // static const std::string texturesNameList[] = {"None", "Water", "Grass", "Rocks1", "Rocks2", "Sandy Grass", "Stony Ground", "Snow"};
    static const std::string texturesNameList[] = {"None",
                                                   "dirt",
                                                   "grass-2",
                                                   "grass-2",
                                                   "grass",
                                                   "rock-snow-ice-",
                                                   "rocks1",
                                                   "rocks2",
                                                   "rough-wet-cobble-albedo",
                                                   "sandy-grass",
                                                   "sandy-rocks",
                                                   "sandyground",
                                                   "snow-packed",
                                                   "snow",
                                                   "stony-ground",
                                                   "water",
                                                   "worn-bumpy-rock"};

    static const int arrSize = IM_ARRAYSIZE(texturesNameList);
    const auto &texturesList = ResourceManager::GetTexturesList();

    if (ImGui::BeginTabItem("Colors"))
    {
        ImGui::SliderInt("Layers", &s_Props.NumOfTextureLayers, 0, 8);

        for (int i = 0; i < s_Props.NumOfTextureLayers; i++)
        {
            std::string index = std::to_string(i + 1);
            std::string uiName = "Layer " + index;
            ImGui::PushID(uiName.c_str());
            ImGui::SeparatorText(uiName.c_str());

            {
                ImGui::ColorEdit3("Color", glm::value_ptr(s_Props.TextureLayers[i].Color));
                ImGui::SliderFloat("Color Strength", &s_Props.TextureLayers[i].ColorStrength, 0.0f, 1.0f);
                ImGui::SliderFloat("Blend Strength", &s_Props.TextureLayers[i].BlendBoundaries, 0.0f, 1.0f);
                ImGui::SliderFloat("Layer Blend", &s_Props.TextureLayers[i].BlendLayer, 0.0f, 1.0f);
                ImGui::DragFloat("Texture Scale", &s_Props.TextureLayers[i].TextureScale, 0.001f, 0.001f, 1.0f);

                ImGui::Separator();
                {
                    // Slope options

                    ImGui::DragFloatRange2("Slope Height Range", &s_Props.TextureLayers[i].SlopeHeightBegin, &s_Props.TextureLayers[i].SlopeHeightEnd, 0.001f, 0.0f, 1.0f, "Min: %.3f", "Max: %.3f", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::SliderFloat("Slope Threshold", &s_Props.TextureLayers[i].SlopeThreshold, 0.0f, 1.0f);
                    ImGui::SliderFloat("Slope Blend", &s_Props.TextureLayers[i].BlendSlope, 0.0f, 1.0f);
                }
                ImGui::Separator();

                const std::string dragDropPayloadName = "Terrain Texture Index";
                ImGui::BeginGroup();
                if (s_Props.TextureLayers[i].TextureIndex == 0)
                {
                    ImGui::Button(texturesNameList[s_Props.TextureLayers[i].TextureIndex].c_str(), ImVec2(80, 80));
                }
                else
                {
                    const intptr_t textureId = texturesList[s_Props.TextureLayers[i].TextureIndex - 1]->GetRendererId();
                    ImGui::Image((void *)textureId, ImVec2(80, 80));
                }
                ImGui::EndGroup();

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(dragDropPayloadName.c_str()))
                    {
                        int payloadIndex = *(const int *)payload->Data;
                        s_Props.TextureLayers[i].TextureIndex = payloadIndex;
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::SameLine();
                if (ImGui::Button("X", ImVec2(20, 20)))
                {
                    s_Props.TextureLayers[i].TextureIndex = 0;
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
                    ImGui::Text("%s", texturesNameList[i].c_str());
                }
                ImGui::EndGroup();

                float lastButtonX = ImGui::GetItemRectMax().x;
                float nextButtonX = lastButtonX + itemHorizontalSpacing + buttonSize.x;
                if (count + 1 < arrSize && nextButtonX < windowVisibleX)
                    ImGui::SameLine();

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    ImGui::SetDragDropPayload(dragDropPayloadName.c_str(), &i, sizeof(int));
                    ImGui::Text("%s", texturesNameList[i].c_str());
                    ImGui::EndDragDropSource();
                }
            }

            ImGui::PopID();
        }
        ImGui::PopStyleVar();
        ImGui::End();
    }
}
