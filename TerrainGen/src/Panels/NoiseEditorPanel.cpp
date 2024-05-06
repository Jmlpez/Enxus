#include "pch.h"
#include "NoiseEditorPanel.h"

NoiseEditorPanel::NoiseEditorPanel()
{
    m_NoiseTexture = Enxus::CreateScope<Enxus::Texture2D>(m_PreviewSize[0], m_PreviewSize[1]);
}

void NoiseEditorPanel::OnImGuiRender()
{
    static const char *enumNoiseType[] = {"OpenSimplex2", "OpenSimplex2S", "Cellular", "Perlin", "Value Cubic", "Value"};
    static const char *enumFractalType[] = {"None", "FBm", "Ridged", "Ping Pong"};
    static const char *enumCellularType[] = {"Euclidean", "Euclidean Sq", "Manhattan", "Hybrid"};
    static const char *enumCellularReturnType[] = {"Cell Value", "Distance", "Distance 2", "Distance 2 Add", "Distance 2 Sub", "Distance 2 Mul", "Distance 2 Div"};
    static const char *enumDomainWarpType[] = {"None", "OpenSimplex2", "OpenSimplex2 Reduced", "Basic Grid"};
    static const char *enumDomainWarpFractalType[] = {"None", "Progressive", "Independent"};

    m_IsNewTexture = false;

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
                m_IsNewTexture = true;
            }

            if (ImGui::DragInt("Seed", &m_GeneralNoise.Seed))
            {
                m_Fnl.SetSeed(m_GeneralNoise.Seed);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Frequency", &m_GeneralNoise.Frequency, 0.0002f))
            {
                m_Fnl.SetFrequency(m_GeneralNoise.Frequency);
                m_IsNewTexture = true;
            }
        }

        //----------------- FRACTAL -------------------//
        {
            ImGui::TextUnformatted("Fractal");

            if (ImGui::Combo("Type", &m_FractalNoise.Type, enumFractalType, IM_ARRAYSIZE(enumFractalType)))
            {
                m_Fnl.SetFractalType((FastNoiseLite::FractalType)m_FractalNoise.Type);
                m_IsNewTexture = true;
            }
            ImGui::BeginDisabled(m_FractalNoise.Type == 0);
            if (ImGui::DragInt("Octaves", &m_FractalNoise.Octaves, 0.1f, 1, 20))
            {
                m_Fnl.SetFractalOctaves(m_FractalNoise.Octaves);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Lacunarity", &m_FractalNoise.Lacunarity, 0.01f))
            {
                m_Fnl.SetFractalLacunarity(m_FractalNoise.Lacunarity);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Gain", &m_FractalNoise.Gain, 0.01f))
            {
                m_Fnl.SetFractalGain(m_FractalNoise.Gain);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Weighted Strength", &m_FractalNoise.WeightedStrength, 0.01f))
            {
                m_Fnl.SetFractalWeightedStrength(m_FractalNoise.WeightedStrength);
                m_IsNewTexture = true;
            }
            ImGui::BeginDisabled(m_FractalNoise.Type != (int)FastNoiseLite::FractalType_PingPong);
            if (ImGui::DragFloat("Ping Pong Strength", &m_FractalNoise.PingPongStrength, 0.01f))
            {
                m_Fnl.SetFractalPingPongStrength(m_FractalNoise.PingPongStrength);
                m_IsNewTexture = true;
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
                m_IsNewTexture = true;
            }
            if (ImGui::Combo("Return Type", &m_CellularNoise.ReturnType, enumCellularReturnType, IM_ARRAYSIZE(enumCellularReturnType)))
            {
                m_Fnl.SetCellularReturnType((FastNoiseLite::CellularReturnType)m_CellularNoise.ReturnType);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Jitter", &m_CellularNoise.Jitter, 0.01f))
            {
                m_Fnl.SetCellularJitter(m_CellularNoise.Jitter);
                m_IsNewTexture = true;
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
                m_IsNewTexture = true;
            }
            ImGui::BeginDisabled(m_DomainWarp.Type == 0);

            if (ImGui::DragFloat("Amplitude", &m_DomainWarp.Amplitude, 0.5f))
            {
                m_FnlWarp.SetDomainWarpAmp(m_DomainWarp.Amplitude);
                m_IsNewTexture = true;
            }
            if (ImGui::DragInt("Seed", &m_DomainWarp.Seed))
            {
                m_FnlWarp.SetSeed(m_DomainWarp.Seed);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Frequency", &m_DomainWarp.Frequency, 0.001f))
            {
                m_FnlWarp.SetFrequency(m_DomainWarp.Frequency);
                m_IsNewTexture = true;
            }

            /// Domain Warp Fractal
            ImGui::PushID("Domain Warp Fractal");
            ImGui::TextUnformatted("Domain Warp Fractal");

            if (ImGui::Combo("Type", &m_DomainWarpFractal.Type, enumDomainWarpFractalType, IM_ARRAYSIZE(enumDomainWarpFractalType)))
            {
                m_FnlWarp.SetFractalType((FastNoiseLite::FractalType)(m_DomainWarpFractal.Type ? m_DomainWarpFractal.Type + 3 : 0));
                m_IsNewTexture = true;
            }
            ImGui::BeginDisabled(m_DomainWarpFractal.Type == 0);
            if (ImGui::DragInt("Octaves", &m_DomainWarpFractal.Octaves, 0.1f, 1, 20))
            {
                m_FnlWarp.SetFractalOctaves(m_DomainWarpFractal.Octaves);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Lacunarity", &m_DomainWarpFractal.Lacunarity, 0.01f))
            {
                m_FnlWarp.SetFractalLacunarity(m_DomainWarpFractal.Lacunarity);
                m_IsNewTexture = true;
            }
            if (ImGui::DragFloat("Gain", &m_DomainWarpFractal.Gain, 0.01f))
            {
                m_FnlWarp.SetFractalGain(m_DomainWarpFractal.Gain);
                m_IsNewTexture = true;
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
        ImGui::Checkbox("Auto Size", &m_PreviewAutoSize);
        ImGui::BeginDisabled(m_PreviewAutoSize);
        ImGui::DragInt2("Size", &m_TextureData.ImGuiTexSize[0], 1, 32, 4096);
        ImGui::EndDisabled();
        if (ImGui::DragFloat("Black Point", &m_TextureData.ColorTexMin, 0.01f))
        {
            m_IsNewTexture = true;
        }
        if (ImGui::DragFloat("White Point", &m_TextureData.ColorTexMax, 0.01f))
        {
            m_IsNewTexture = true;
        }

        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::PopItemWidth();
    ImGui::End();

    {
        ImGui::Begin("Noise Texture");

        if (m_PreviewAutoSize)
        {
            ImVec2 autoSize = ImGui::GetContentRegionAvail();
            m_PreviewSize[0] = autoSize.x;
            m_PreviewSize[1] = autoSize.y;
        }

        if (m_TextureData.TexSizeGenX != m_PreviewSize[0] || m_TextureData.TexSizeGenY != m_PreviewSize[1])
        {
            m_IsNewTexture = true;
        }

        UpdateTexture(m_IsNewTexture);

        ImGui::Image((void *)(intptr_t)m_NoiseTexture->GetRendererId(), ImVec2(m_TextureData.ImGuiTexSize[0], m_TextureData.ImGuiTexSize[1]));
        ImGui::End();
    }
}

void NoiseEditorPanel::UpdateTexture(bool newPreview)
{
    if (!newPreview)
    {
        return;
    }

    if (m_PreviewPixelArray)
    {
        delete[] m_PreviewPixelArray;
        m_NoiseMapArray.clear();
    }

    m_TextureData.TexSizeGenX = std::max(0, m_PreviewSize[0]);
    m_TextureData.TexSizeGenY = std::max(0, m_PreviewSize[1]);

    const uint32_t fullSize = m_TextureData.TexSizeGenX * m_TextureData.TexSizeGenY;

    m_PreviewPixelArray = new unsigned char[fullSize * 4];
    if (fullSize > m_NoiseMapArray.capacity())
        m_NoiseMapArray.reserve(fullSize);

    // For the colors
    float scale = 255.0f / (m_TextureData.ColorTexMax - m_TextureData.ColorTexMin);

    int pixelArrayIndex = 0;
    for (int y = 0; y < m_TextureData.TexSizeGenY; y++)
    {
        for (int x = 0; x < m_TextureData.TexSizeGenX; x++)
        {
            float noise;
            double posX = (double)(x - m_TextureData.TexSizeGenX / 2);
            double posY = (double)(y - m_TextureData.TexSizeGenY / 2);

            if (m_DomainWarp.Type > 0)
            {
                m_FnlWarp.DomainWarp(posX, posY);
            }
            noise = m_Fnl.GetNoise(posX, posY);

            unsigned char cNoise = (unsigned char)std::max(0.0f, std::min(255.0f, (noise - m_TextureData.ColorTexMin) * scale));
            m_PreviewPixelArray[pixelArrayIndex++] = cNoise;
            m_PreviewPixelArray[pixelArrayIndex++] = cNoise;
            m_PreviewPixelArray[pixelArrayIndex++] = cNoise;
            m_PreviewPixelArray[pixelArrayIndex++] = 255;

            m_NoiseMapArray.emplace_back(noise);
        }
    }

    m_TextureData.ImGuiTexSize[0] = m_TextureData.TexSizeGenX;
    m_TextureData.ImGuiTexSize[1] = m_TextureData.TexSizeGenY;

    m_NoiseTexture->Resize(m_TextureData.TexSizeGenX, m_TextureData.TexSizeGenY);
    m_NoiseTexture->SetData(m_PreviewPixelArray, m_TextureData.TexSizeGenX, m_TextureData.TexSizeGenY);
}