#include "pch.h"
#include "NoiseEditorPanel.h"

const int NoiseEditorPanel::NoiseTexturePreview::s_TextureGeneratedWidth = 400;
const int NoiseEditorPanel::NoiseTexturePreview::s_TextureGeneratedHeight = 400;

NoiseEditorPanel::NoiseEditorPanel()
{
    m_GeneralNoise.Width = 250;
    m_GeneralNoise.Height = 250;

    m_NoisePreviewData.Texture =
        Enxus::CreateScope<Enxus::Texture2D>(m_NoisePreviewData.s_TextureGeneratedWidth,
                                             m_NoisePreviewData.s_TextureGeneratedHeight);

    // Generate the first image
    UpdateNoiseMap(true);
}

void NoiseEditorPanel::OnImGuiRender()
{
    static const char *enumNoiseType[] = {"OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "Value Cubic", "Value"};
    static const char *enumFractalType[] = {"None", "FBm", "Ridged", "Ping Pong"};
    static const char *enumCellularType[] = {"Euclidean", "Euclidean Sq", "Manhattan", "Hybrid"};
    static const char *enumCellularReturnType[] = {"Cell Value", "Distance", "Distance 2", "Distance 2 Add", "Distance 2 Sub", "Distance 2 Mul", "Distance 2 Div"};
    static const char *enumDomainWarpType[] = {"None", "OpenSimplex2", "OpenSimplex2 Reduced", "Basic Grid"};
    static const char *enumDomainWarpFractalType[] = {"None", "Progressive", "Independent"};

    m_NoiseUpdateFlag = false;

    ImGui::Begin("Noise Generator Panel");
    ImGui::PushItemWidth(120);

    ImGui::BeginTabBar("Tabs");
    if (ImGui::BeginTabItem("FastNoise Lite"))
    {

        //----------------- GENERAL -------------------//
        {
            ImGui::TextUnformatted("General");
            if (ImGui::Combo("Noise Type", &m_GeneralNoise.NoiseType, enumNoiseType, IM_ARRAYSIZE(enumNoiseType)))
            {
                m_Fnl.SetNoiseType((FastNoiseLite::NoiseType)m_GeneralNoise.NoiseType);
                m_NoiseUpdateFlag = true;
            }

            if (ImGui::DragInt("Seed", &m_GeneralNoise.Seed))
            {
                m_Fnl.SetSeed(m_GeneralNoise.Seed);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Frequency", &m_GeneralNoise.Frequency, 0.0002f))
            {
                m_Fnl.SetFrequency(m_GeneralNoise.Frequency);
                m_NoiseUpdateFlag = true;
            }
        }

        //----------------- FRACTAL -------------------//
        {
            ImGui::TextUnformatted("Fractal");

            if (ImGui::Combo("Type", &m_FractalNoise.Type, enumFractalType, IM_ARRAYSIZE(enumFractalType)))
            {
                m_Fnl.SetFractalType((FastNoiseLite::FractalType)m_FractalNoise.Type);
                m_NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(m_FractalNoise.Type == 0);
            if (ImGui::DragInt("Octaves", &m_FractalNoise.Octaves, 0.1f, 1, 20))
            {
                m_Fnl.SetFractalOctaves(m_FractalNoise.Octaves);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Lacunarity", &m_FractalNoise.Lacunarity, 0.01f))
            {
                m_Fnl.SetFractalLacunarity(m_FractalNoise.Lacunarity);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Gain", &m_FractalNoise.Gain, 0.01f))
            {
                m_Fnl.SetFractalGain(m_FractalNoise.Gain);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Weighted Strength", &m_FractalNoise.WeightedStrength, 0.01f))
            {
                m_Fnl.SetFractalWeightedStrength(m_FractalNoise.WeightedStrength);
                m_NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(m_FractalNoise.Type != (int)FastNoiseLite::FractalType_PingPong);
            if (ImGui::DragFloat("Ping Pong Strength", &m_FractalNoise.PingPongStrength, 0.01f))
            {
                m_Fnl.SetFractalPingPongStrength(m_FractalNoise.PingPongStrength);
                m_NoiseUpdateFlag = true;
            }
            ImGui::EndDisabled();

            ImGui::EndDisabled();
        }

        //----------------- CELLULAR -------------------//

        {
            ImGui::TextUnformatted("Cellular");

            ImGui::BeginDisabled(m_GeneralNoise.NoiseType != (int)FastNoiseLite::NoiseType_Cellular);
            if (ImGui::Combo("Distance Function", &m_CellularNoise.Type, enumCellularType, IM_ARRAYSIZE(enumCellularType)))
            {
                m_Fnl.SetCellularDistanceFunction((FastNoiseLite::CellularDistanceFunction)m_CellularNoise.Type);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::Combo("Return Type", &m_CellularNoise.ReturnType, enumCellularReturnType, IM_ARRAYSIZE(enumCellularReturnType)))
            {
                m_Fnl.SetCellularReturnType((FastNoiseLite::CellularReturnType)m_CellularNoise.ReturnType);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Jitter", &m_CellularNoise.Jitter, 0.01f))
            {
                m_Fnl.SetCellularJitter(m_CellularNoise.Jitter);
                m_NoiseUpdateFlag = true;
            }
            ImGui::EndDisabled();
        }

        //----------------- DOMAIN WARP -------------------//
        {
            ImGui::PushID("Domain Warp");
            ImGui::TextUnformatted("Domain Warp");

            if (ImGui::Combo("Type", &m_DomainWarp.Type, enumDomainWarpType, IM_ARRAYSIZE(enumDomainWarpType)))
            {
                m_FnlWarp.SetDomainWarpType((FastNoiseLite::DomainWarpType)(m_DomainWarp.Type - 1));
                m_NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(m_DomainWarp.Type == 0);

            if (ImGui::DragFloat("Amplitude", &m_DomainWarp.Amplitude, 0.5f))
            {
                m_FnlWarp.SetDomainWarpAmp(m_DomainWarp.Amplitude);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragInt("Seed", &m_DomainWarp.Seed))
            {
                m_FnlWarp.SetSeed(m_DomainWarp.Seed);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Frequency", &m_DomainWarp.Frequency, 0.001f))
            {
                m_FnlWarp.SetFrequency(m_DomainWarp.Frequency);
                m_NoiseUpdateFlag = true;
            }

            /// Domain Warp Fractal
            ImGui::PushID("Domain Warp Fractal");
            ImGui::TextUnformatted("Domain Warp Fractal");

            if (ImGui::Combo("Type", &m_DomainWarpFractal.Type, enumDomainWarpFractalType, IM_ARRAYSIZE(enumDomainWarpFractalType)))
            {
                m_FnlWarp.SetFractalType((FastNoiseLite::FractalType)(m_DomainWarpFractal.Type ? m_DomainWarpFractal.Type + 3 : 0));
                m_NoiseUpdateFlag = true;
            }
            ImGui::BeginDisabled(m_DomainWarpFractal.Type == 0);
            if (ImGui::DragInt("Octaves", &m_DomainWarpFractal.Octaves, 0.1f, 1, 20))
            {
                m_FnlWarp.SetFractalOctaves(m_DomainWarpFractal.Octaves);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Lacunarity", &m_DomainWarpFractal.Lacunarity, 0.01f))
            {
                m_FnlWarp.SetFractalLacunarity(m_DomainWarpFractal.Lacunarity);
                m_NoiseUpdateFlag = true;
            }
            if (ImGui::DragFloat("Gain", &m_DomainWarpFractal.Gain, 0.01f))
            {
                m_FnlWarp.SetFractalGain(m_DomainWarpFractal.Gain);
                m_NoiseUpdateFlag = true;
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
    if (ImGui::BeginTabItem("Preview Settings"))
    {
        ImGui::Checkbox("Auto Size", &m_NoisePreviewData.IsAutoSize);
        ImGui::BeginDisabled(m_NoisePreviewData.IsAutoSize);
        ImGui::DragInt("Width", &m_NoisePreviewData.ImGuiWidth, 1, 32, 2048);
        ImGui::DragInt("Height", &m_NoisePreviewData.ImGuiHeight, 1, 32, 2048);
        ImGui::EndDisabled();
        if (ImGui::DragFloat("Black Point", &m_NoisePreviewData.ColorTexMin, 0.01f))
        {
            m_NoiseUpdateFlag = true;
        }
        if (ImGui::DragFloat("White Point", &m_NoisePreviewData.ColorTexMax, 0.01f))
        {
            m_NoiseUpdateFlag = true;
        }

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::PopItemWidth();
    ImGui::End();

    {
        ImGui::Begin("Noise Texture");

        if (m_NoisePreviewData.IsAutoSize)
        {
            ImVec2 autoSize = ImGui::GetContentRegionAvail();
            int minSize = std::min(autoSize.x, autoSize.y);
            m_NoisePreviewData.ImGuiWidth = m_NoisePreviewData.ImGuiHeight = minSize;
        }

        // if (m_NoisePreviewData.Width != m_PreviewSize[0] || m_NoisePreviewData.Height != m_PreviewSize[1])
        // {
        //     m_NoiseUpdateFlag = true;
        // }

        UpdateNoiseMap(m_NoiseUpdateFlag);
        ImGui::Image((void *)(intptr_t)m_NoisePreviewData.Texture->GetRendererId(), ImVec2(m_NoisePreviewData.ImGuiWidth, m_NoisePreviewData.ImGuiHeight));
        ImGui::End();
    }
}

void NoiseEditorPanel::UpdateNoiseMap(bool newPreview)
{
    if (!newPreview)
    {
        return;
    }

    m_NoiseMapArray.clear();

    const uint32_t noiseFullSize = m_GeneralNoise.Width * m_GeneralNoise.Height;
    if (noiseFullSize > m_NoiseMapArray.capacity())
        m_NoiseMapArray.reserve(noiseFullSize);

    for (int y = 0; y < m_GeneralNoise.Height; y++)
    {
        for (int x = 0; x < m_GeneralNoise.Width; x++)
        {
            float noise;
            double posX = (double)(x - m_GeneralNoise.Width / 2);
            double posY = (double)(y - m_GeneralNoise.Height / 2);

            if (m_DomainWarp.Type > 0)
            {
                m_FnlWarp.DomainWarp(posX, posY);
            }
            noise = m_Fnl.GetNoise(posX, posY);
            m_NoiseMapArray.emplace_back(noise);
        }
    }

    // For the colors
    float scale = 255.0f / (m_NoisePreviewData.ColorTexMax - m_NoisePreviewData.ColorTexMin);

    // to make easy to type..
    int imgWidth = m_NoisePreviewData.s_TextureGeneratedWidth;
    int imgHeight = m_NoisePreviewData.s_TextureGeneratedHeight;

    unsigned char *previewPixelArray = new unsigned char[imgWidth * imgHeight * 4];

    int pixelArrayIndex = 0;
    for (int y = 0; y < imgHeight; y++)
    {
        for (int x = 0; x < imgWidth; x++)
        {
            float noise;
            double posX = (double)(x - imgWidth / 2);
            double posY = (double)(y - imgHeight / 2);

            if (m_DomainWarp.Type > 0)
            {
                m_FnlWarp.DomainWarp(posX, posY);
            }
            noise = m_Fnl.GetNoise(posX, posY);

            unsigned char cNoise = (unsigned char)std::max(0.0f, std::min(255.0f, (noise - m_NoisePreviewData.ColorTexMin) * scale));
            previewPixelArray[pixelArrayIndex++] = cNoise;
            previewPixelArray[pixelArrayIndex++] = cNoise;
            previewPixelArray[pixelArrayIndex++] = cNoise;
            previewPixelArray[pixelArrayIndex++] = 255;
        }
    }

    // m_NoisePreviewData.ImGuiTexSize[0] = m_NoisePreviewData.Width;
    // m_NoisePreviewData.ImGuiTexSize[1] = m_NoisePreviewData.Height;

    m_NoisePreviewData.Texture->Resize(imgWidth, imgHeight);
    m_NoisePreviewData.Texture->SetData(previewPixelArray, imgWidth, imgHeight);

    delete[] previewPixelArray;
}