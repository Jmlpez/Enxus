#ifndef NOISE_MAP_GENERATOR_H
#define NOISE_MAP_GENERATOR_H

#include "Enxus.h"
#include "SimpleNoise.h"

class NoiseMapGenerator
{
public:
    NoiseMapGenerator(uint32_t width, uint32_t height);
    ~NoiseMapGenerator();

    Enxus::Ref<Enxus::Texture2D> GetNoiseMapTexture();

    inline float At(int i, int j) const { return m_NoiseMap[i][j]; }

    inline uint32_t GetWidth() const { return m_Width; }
    inline uint32_t GetHeight() const { return m_Height; }

    // setters... freq, ampl, lac, pers, width, height...
    void SetFrequency(float frequency);
    void SetAmplitude(float amplitude);
    void SetLacuranity(float lacuranity);
    void SetPersistance(float persistance);

private:
    void CalculateNoise();
    void GenerateImage();

private:
    static const uint32_t c_MaxSize = 2500;

    uint32_t m_Width, m_Height;
    float m_NoiseMap[c_MaxSize][c_MaxSize];

    float m_Frequency = 0.025f, m_Lacuranity = 2.0f;
    float m_Amplitude = 2.0f, m_Persistance = 0.5f;
    float m_MaxNoiseVal = 0.0f;

    uint32_t m_NumOfLayers = 5;

    Enxus::Ref<Enxus::Texture2D> m_NoiseMapTexture;

    // simple noise generator
    SimpleNoise m_SimpleNoise;
};

#endif