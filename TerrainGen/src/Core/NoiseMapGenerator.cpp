#include "pch.h"
#include "NoiseMapGenerator.h"
#include "SimpleNoise.h"

NoiseMapGenerator::NoiseMapGenerator(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height)
{
    m_NoiseMapTexture = Enxus::CreateRef<Enxus::Texture2D>(width, height);
    m_SimpleNoise.Init();
    CalculateNoise();
}

NoiseMapGenerator::~NoiseMapGenerator()
{
}

Enxus::Ref<Enxus::Texture2D> NoiseMapGenerator::GetNoiseMapTexture()
{
    return m_NoiseMapTexture;
}

void NoiseMapGenerator::SetFrequency(float frequency)
{
    if (frequency == m_Frequency)
        return;
    m_Frequency = frequency;
    CalculateNoise();
}
void NoiseMapGenerator::SetAmplitude(float amplitude)
{
    if (amplitude == m_Amplitude)
        return;
    m_Amplitude = amplitude;
    CalculateNoise();
}
void NoiseMapGenerator::SetLacuranity(float lacuranity)
{
    if (lacuranity == m_Lacuranity)
        return;
    m_Lacuranity = lacuranity;
    CalculateNoise();
}
void NoiseMapGenerator::SetPersistance(float persistance)
{
    if (persistance == m_Persistance)
        return;
    m_Persistance = persistance;
    CalculateNoise();
}

void NoiseMapGenerator::CalculateNoise()
{

    m_MaxNoiseVal = 0.0f;
    for (uint32_t j = 0; j < m_Height; ++j)
    {
        for (uint32_t i = 0; i < m_Width; ++i)
        {
            glm::vec2 point(i, j);
            float frequency = m_Frequency, amplitude = m_Amplitude;

            m_NoiseMap[i][j] = 0.0f; // restart values

            for (uint32_t k = 0; k < m_NumOfLayers; k++)
            {
                m_NoiseMap[i][j] += m_SimpleNoise.Eval(point * frequency) * amplitude;

                frequency *= m_Lacuranity;
                amplitude *= m_Persistance;
            }
            if (m_NoiseMap[i][j] > m_MaxNoiseVal)
                m_MaxNoiseVal = m_NoiseMap[i][j];
        }
    }
    GenerateImage();
}

void NoiseMapGenerator::GenerateImage()
{
    unsigned char *colorData = new unsigned char[m_Height * m_Width * 4];
    for (uint32_t j = 0; j < m_Height; ++j)
    {
        for (uint32_t i = 0; i < m_Width; ++i)
        {
            m_NoiseMap[i][j] /= m_MaxNoiseVal; // because teh color is in range (0-255)
            int color = (m_NoiseMap[i][j] * 255);

            for (int k = 0; k < 4; k++)
            {
                int offset = 4 * (j * m_Width + i) + k;
                colorData[offset] = (k != 4) ? color : 255;
            }
        }
    }
    // set the noise mpa data to the texture2D
    m_NoiseMapTexture->SetData(colorData, m_Width, m_Height);
    delete[] colorData;
}