#include "pch.h"
#include "NoiseEditorPanel.h"
#include "imgui/imgui.h"

static NoiseEditorPanelProps s_Props;

void NoiseEditorPanel::Init()
{
    srand(time(nullptr));
    s_Props.GeneralNoise.Seed = rand() % 1000 + 1000;
    s_Props.Fnl.SetSeed(s_Props.GeneralNoise.Seed);

    s_Props.NoisePreviewData.Texture =
        Enxus::CreateScope<Enxus::Texture2D>(s_Props.NoiseWidth,
                                             s_Props.NoiseHeight);
    s_Props.FalloffMap.Texture =
        Enxus::CreateScope<Enxus::Texture2D>(s_Props.NoiseWidth,
                                             s_Props.NoiseHeight);

    // Just one memory allocation
    s_Props.NoiseMapArray.reserve(s_Props.NoiseWidth * s_Props.NoiseHeight);
    s_Props.NoiseMapArray.assign(s_Props.NoiseWidth * s_Props.NoiseHeight, 0.0f);

    // Generate the first image
    UpdateNoiseMap(true);
}

void NoiseEditorPanel::ShutDown()
{
}

bool NoiseEditorPanel::HasUpdated() { return s_Props.NoiseUpdateFlag; }

uint32_t NoiseEditorPanel::GetNoiseWidth()
{
    return (uint32_t)s_Props.NoiseWidth;
}
uint32_t NoiseEditorPanel::GetNoiseHeight()
{
    return (uint32_t)s_Props.NoiseHeight;
}

const std::vector<float> &NoiseEditorPanel::GetNoiseMap()
{
    return s_Props.NoiseMapArray;
}

void NoiseEditorPanel::SetNoiseWidth(uint32_t width)
{
    (void)width;
    std::cout << "[NoiseEditorPanel Error] : SetNoiseWidth Currently not supported" << std::endl;
    return;
    // if ((int)width == s_Props.GeneralNoise.Width)
    //     return;
    // s_Props.GeneralNoise.Width = width;
    // UpdateNoiseMap(true);
}
void NoiseEditorPanel::SetNoiseHeight(uint32_t height)
{
    (void)height;
    std::cout << "[NoiseEditorPanel Error] : SetNoiseHeight Currently not supported" << std::endl;
    return;
    // if ((int)height == s_Props.GeneralNoise.Height)
    //     return;
    // s_Props.GeneralNoise.Height = height;
    // UpdateNoiseMap(true);
}

void NoiseEditorPanel::OnImGuiRender()
{
    static const char *enumNoiseType[] = {"OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "Value Cubic", "Value"};
    static const char *enumFractalType[] = {"None", "FBm", "Ridged", "Ping Pong"};
    static const char *enumCellularType[] = {"Euclidean", "Euclidean Sq", "Manhattan", "Hybrid"};
    static const char *enumCellularReturnType[] = {"Cell Value", "Distance", "Distance 2", "Distance 2 Add", "Distance 2 Sub", "Distance 2 Mul", "Distance 2 Div"};
    static const char *enumDomainWarpType[] = {"None", "OpenSimplex2", "OpenSimplex2 Reduced", "Basic Grid"};
    static const char *enumDomainWarpFractalType[] = {"None", "Progressive", "Independent"};

    s_Props.NoiseUpdateFlag = false;

    ImGui::Begin("Noise Generator Panel");
    ImGui::PushItemWidth(120);

    ImGui::BeginTabBar("Tabs");
    if (ImGui::BeginTabItem("FastNoise Lite"))
    {

        //----------------- GENERAL -------------------//
        {
            ImGui::TextUnformatted("General");
            if (ImGui::Combo("Noise Type", &s_Props.GeneralNoise.NoiseType, enumNoiseType, IM_ARRAYSIZE(enumNoiseType)))
            {
                s_Props.Fnl.SetNoiseType((FastNoiseLite::NoiseType)s_Props.GeneralNoise.NoiseType);
                s_Props.NoiseUpdateFlag = true;
            }

            if (ImGui::Checkbox("Invert Noise", &s_Props.GeneralNoise.IsInverted))
            {
                s_Props.NoiseUpdateFlag = true;
            }

            if (ImGui::DragInt("Seed", &s_Props.GeneralNoise.Seed))
            {
                s_Props.Fnl.SetSeed(s_Props.GeneralNoise.Seed);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Frequency", &s_Props.GeneralNoise.Frequency, 0.0001f))
            {
                s_Props.Fnl.SetFrequency(s_Props.GeneralNoise.Frequency);
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::PushItemWidth(50);
            if (ImGui::DragFloat("Offset X", &s_Props.GeneralNoise.OffsetX, 1.0f))
            {
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::SameLine();
            if (ImGui::DragFloat("Offset Y", &s_Props.GeneralNoise.OffsetY, 1.0f))
            {
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::PopItemWidth();
        }

        //----------------- FRACTAL -------------------//
        {
            ImGui::TextUnformatted("Fractal");

            if (ImGui::Combo("Type", &s_Props.FractalNoise.Type, enumFractalType, IM_ARRAYSIZE(enumFractalType)))
            {
                s_Props.Fnl.SetFractalType((FastNoiseLite::FractalType)s_Props.FractalNoise.Type);
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(s_Props.FractalNoise.Type == 0);
            if (ImGui::DragInt("Octaves", &s_Props.FractalNoise.Octaves, 0.1f, 1, 20))
            {
                s_Props.Fnl.SetFractalOctaves(s_Props.FractalNoise.Octaves);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Lacunarity", &s_Props.FractalNoise.Lacunarity, 0.01f))
            {
                s_Props.Fnl.SetFractalLacunarity(s_Props.FractalNoise.Lacunarity);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Gain", &s_Props.FractalNoise.Gain, 0.001f))
            {
                s_Props.Fnl.SetFractalGain(s_Props.FractalNoise.Gain);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Weighted Strength", &s_Props.FractalNoise.WeightedStrength, 0.01f))
            {
                s_Props.Fnl.SetFractalWeightedStrength(s_Props.FractalNoise.WeightedStrength);
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(s_Props.FractalNoise.Type != (int)FastNoiseLite::FractalType_PingPong);
            if (ImGui::DragFloat("Ping Pong Strength", &s_Props.FractalNoise.PingPongStrength, 0.01f))
            {
                s_Props.Fnl.SetFractalPingPongStrength(s_Props.FractalNoise.PingPongStrength);
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::EndDisabled();

            ImGui::EndDisabled();
        }

        //----------------- CELLULAR -------------------//

        {
            ImGui::TextUnformatted("Cellular");

            ImGui::BeginDisabled(s_Props.GeneralNoise.NoiseType != (int)FastNoiseLite::NoiseType_Cellular);
            if (ImGui::Combo("Distance Function", &s_Props.CellularNoise.Type, enumCellularType, IM_ARRAYSIZE(enumCellularType)))
            {
                s_Props.Fnl.SetCellularDistanceFunction((FastNoiseLite::CellularDistanceFunction)s_Props.CellularNoise.Type);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::Combo("Return Type", &s_Props.CellularNoise.ReturnType, enumCellularReturnType, IM_ARRAYSIZE(enumCellularReturnType)))
            {
                s_Props.Fnl.SetCellularReturnType((FastNoiseLite::CellularReturnType)s_Props.CellularNoise.ReturnType);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Jitter", &s_Props.CellularNoise.Jitter, 0.01f))
            {
                s_Props.Fnl.SetCellularJitter(s_Props.CellularNoise.Jitter);
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::EndDisabled();
        }

        //----------------- DOMAIN WARP -------------------//
        {
            ImGui::PushID("Domain Warp");
            ImGui::TextUnformatted("Domain Warp");

            if (ImGui::Combo("Type", &s_Props.DomainWarp.Type, enumDomainWarpType, IM_ARRAYSIZE(enumDomainWarpType)))
            {
                s_Props.FnlWarp.SetDomainWarpType((FastNoiseLite::DomainWarpType)(s_Props.DomainWarp.Type - 1));
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(s_Props.DomainWarp.Type == 0);

            if (ImGui::DragFloat("Amplitude", &s_Props.DomainWarp.Amplitude, 0.5f))
            {
                s_Props.FnlWarp.SetDomainWarpAmp(s_Props.DomainWarp.Amplitude);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragInt("Seed", &s_Props.DomainWarp.Seed))
            {
                s_Props.FnlWarp.SetSeed(s_Props.DomainWarp.Seed);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Frequency", &s_Props.DomainWarp.Frequency, 0.001f))
            {
                s_Props.FnlWarp.SetFrequency(s_Props.DomainWarp.Frequency);
                s_Props.NoiseUpdateFlag = true;
            }

            /// Domain Warp Fractal
            ImGui::PushID("Domain Warp Fractal");
            ImGui::TextUnformatted("Domain Warp Fractal");

            if (ImGui::Combo("Type", &s_Props.DomainWarpFractal.Type, enumDomainWarpFractalType, IM_ARRAYSIZE(enumDomainWarpFractalType)))
            {
                s_Props.FnlWarp.SetFractalType((FastNoiseLite::FractalType)(s_Props.DomainWarpFractal.Type ? s_Props.DomainWarpFractal.Type + 3 : 0));
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(s_Props.DomainWarpFractal.Type == 0);
            if (ImGui::DragInt("Octaves", &s_Props.DomainWarpFractal.Octaves, 0.1f, 1, 20))
            {
                s_Props.FnlWarp.SetFractalOctaves(s_Props.DomainWarpFractal.Octaves);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Lacunarity", &s_Props.DomainWarpFractal.Lacunarity, 0.01f))
            {
                s_Props.FnlWarp.SetFractalLacunarity(s_Props.DomainWarpFractal.Lacunarity);
                s_Props.NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Gain", &s_Props.DomainWarpFractal.Gain, 0.01f))
            {
                s_Props.FnlWarp.SetFractalGain(s_Props.DomainWarpFractal.Gain);
                s_Props.NoiseUpdateFlag = true;
            }
            ImGui::EndDisabled();
            ImGui::EndDisabled();
            ImGui::PopID();
            ImGui::PopID();
        }
        ImGui::NewLine();
        // ImGui::TextUnformatted(FNL_VERSION);

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Falloff Settings"))
    {
        if (ImGui::Checkbox("Activate", &s_Props.FalloffMap.IsActivated))
        {
            s_Props.NoiseUpdateFlag = true;
        }

        if (ImGui::DragFloat("Beta value", &s_Props.FalloffMap.Beta, 0.1f, 0.1f, 10.0f))
        {
            s_Props.NoiseUpdateFlag = true;
        }

        ImGui::Image((void *)(intptr_t)s_Props.FalloffMap.Texture->GetRendererId(), ImVec2(s_Props.NoisePreviewData.ImGuiWidth, s_Props.NoisePreviewData.ImGuiHeight));

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Preview Settings"))
    {
        ImGui::Checkbox("Auto Size", &s_Props.NoisePreviewData.IsAutoSize);
        ImGui::BeginDisabled(s_Props.NoisePreviewData.IsAutoSize);

        ImGui::PushItemWidth(50);
        ImGui::DragInt("Width", &s_Props.NoisePreviewData.ImGuiWidth, 1, 32, 2048);
        ImGui::SameLine();
        ImGui::DragInt("Height", &s_Props.NoisePreviewData.ImGuiHeight, 1, 32, 2048);
        ImGui::PopItemWidth();
        ImGui::EndDisabled();
        if (ImGui::DragFloat("Black Point", &s_Props.NoisePreviewData.ColorTexMin, 0.01f))
        {
            s_Props.NoiseUpdateFlag = true;
        }
        if (ImGui::DragFloat("White Point", &s_Props.NoisePreviewData.ColorTexMax, 0.01f))
        {
            s_Props.NoiseUpdateFlag = true;
        }

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::PopItemWidth();
    ImGui::End();

    {
        ImGui::Begin("Noise Texture");

        if (s_Props.NoisePreviewData.IsAutoSize)
        {
            ImVec2 autoSize = ImGui::GetContentRegionAvail();
            int minSize = std::min(autoSize.x, autoSize.y);
            s_Props.NoisePreviewData.ImGuiWidth = s_Props.NoisePreviewData.ImGuiHeight = minSize;
        }

        UpdateNoiseMap(s_Props.NoiseUpdateFlag);

        ImGui::Image((void *)(intptr_t)s_Props.NoisePreviewData.Texture->GetRendererId(), ImVec2(s_Props.NoisePreviewData.ImGuiWidth, s_Props.NoisePreviewData.ImGuiHeight));
        ImGui::End();
    }
}

void NoiseEditorPanel::UpdateNoiseMap(bool newMap)
{
    if (!newMap)
        return;

    // For colors
    // The denominator arises from  (1 - (-1))
    // which are the maximun and minimun values of the noise (i.e the noise output is in that range)
    float colorScale = 255.0f / 2.0f;

    uint8_t *noiseMapPixelArray = new uint8_t[s_Props.NoiseWidth * s_Props.NoiseHeight * 4];
    uint8_t *falloffMapPixelArray = new uint8_t[s_Props.NoiseWidth * s_Props.NoiseHeight * 4];

    int pixelArrayIndex = 0;
    int noiseMapIndex = 0;
    for (uint16_t y = 0; y < s_Props.NoiseHeight; y++)
    {
        for (uint16_t x = 0; x < s_Props.NoiseWidth; x++)
        {

            float posX = (float)(x - s_Props.NoiseWidth / 2.0f);
            float posY = (float)(y - s_Props.NoiseHeight / 2.0f);

            if (s_Props.DomainWarp.Type > 0)
            {
                s_Props.FnlWarp.DomainWarp(posX, posY);
            }

            float noise = s_Props.Fnl.GetNoise(posX + s_Props.GeneralNoise.OffsetX, posY + s_Props.GeneralNoise.OffsetY);

            // convert noise from [-1, 1] to [0, 1]
            float normalizedNoise = (noise + 1.0f) * 0.5f;

            uint8_t noiseColor = (uint8_t)std::max(0.0f, std::min(255.0f, (noise + 1.0f) * colorScale));

            // noise texture color data
            noiseMapPixelArray[pixelArrayIndex] = noiseColor;
            noiseMapPixelArray[pixelArrayIndex + 1] = noiseColor;
            noiseMapPixelArray[pixelArrayIndex + 2] = noiseColor;
            noiseMapPixelArray[pixelArrayIndex + 3] = 255;

            if (s_Props.FalloffMap.IsActivated)
            {
                float nPosX = (float)x / (float)s_Props.NoiseWidth * 2.0f - 1;
                float nPosY = (float)y / (float)s_Props.NoiseHeight * 2.0f - 1;

                float falloffValue = std::max(std::abs(nPosX), std::abs(nPosY));
                float smoothValue = Enxus::Math::blend(falloffValue, s_Props.FalloffMap.Beta);

                uint8_t falloffColor = smoothValue * 255;
                falloffMapPixelArray[pixelArrayIndex] = falloffColor;
                falloffMapPixelArray[pixelArrayIndex + 1] = falloffColor;
                falloffMapPixelArray[pixelArrayIndex + 2] = falloffColor;
                falloffMapPixelArray[pixelArrayIndex + 3] = 255;

                normalizedNoise = std::clamp(normalizedNoise - smoothValue, 0.0f, 1.0f);
            }

            s_Props.NoiseMapArray[noiseMapIndex] = normalizedNoise;

            if (s_Props.GeneralNoise.IsInverted)
            {
                s_Props.NoiseMapArray[noiseMapIndex] = 1 - normalizedNoise;
                noiseMapPixelArray[pixelArrayIndex] = 255 - noiseColor;
                noiseMapPixelArray[pixelArrayIndex + 1] = 255 - noiseColor;
                noiseMapPixelArray[pixelArrayIndex + 2] = 255 - noiseColor;
                noiseMapPixelArray[pixelArrayIndex + 3] = 255;
            }

            noiseMapIndex++;
            pixelArrayIndex += 4;
        }
    }
    s_Props.NoisePreviewData.Texture->SetData(noiseMapPixelArray, s_Props.NoiseWidth, s_Props.NoiseHeight);

    if (s_Props.FalloffMap.IsActivated)
    {
        s_Props.FalloffMap.Texture->SetData(falloffMapPixelArray, s_Props.NoiseWidth, s_Props.NoiseHeight);
    }

    delete[] noiseMapPixelArray;
    delete[] falloffMapPixelArray;
}
