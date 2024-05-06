#ifndef NOISE_EDITOR_PANEL_H
#define NOISE_EDITOR_PANEL_H

#include "Enxus.h"
#include "FastNoiseLite.h"
#include "imgui/imgui.h"

class NoiseEditorPanel
{
public:
    NoiseEditorPanel();
    ~NoiseEditorPanel() {}

    void OnImGuiRender();

    inline bool IsUpdated() { return m_IsNewTexture; }
    inline uint32_t GetTextureWidth() { return (uint32_t)m_TextureData.TexSizeGenX; }
    inline uint32_t GetTextureHeight() { return (uint32_t)m_TextureData.TexSizeGenY; }
    inline const std::vector<float> &GetNoiseMap() const { return m_NoiseMapArray; }

private:
    void UpdateTexture(bool newPreview);

private:
    FastNoiseLite m_Fnl;
    FastNoiseLite m_FnlWarp;

    int m_PreviewSize[2] = {250, 250};
    bool m_PreviewDomainWarp = false;
    bool m_PreviewAutoSize = false;

    // To check when update the texture image
    bool m_IsNewTexture;

    // its going to be used only in this class
    unsigned char *m_PreviewPixelArray = nullptr;

    // will be exported to be used in the editor
    std::vector<float> m_NoiseMapArray;

    Enxus::Scope<Enxus::Texture2D> m_NoiseTexture;

    struct NoiseTextureData
    {
        int ImGuiTexSize[2] = {0, 0};
        int TexSizeGenX;
        int TexSizeGenY;
        float ColorTexMin = -1;
        float ColorTexMax = 1;

    } m_TextureData;

    struct GeneralNoiseData
    {
        int NoiseType = 0;
        int Seed = 1337;
        float Frequency = 0.01f;
    } m_GeneralNoise;

    struct FractalNoiseData
    {
        int Type = 0;
        int Octaves = 3;
        float Lacunarity = 2.0f;
        float Gain = 0.5f;
        float WeightedStrength = 0.0f;
        float PingPongStrength = 2.0f;
    } m_FractalNoise;

    struct CellularNoiseData
    {
        int Type = 1;
        int ReturnType = 1;
        float Jitter = 1.0f;
    } m_CellularNoise;

    struct DomainWarpData
    {
        int Seed = 1337;
        float Frequency = 0.01f;
        int Type = 0;
        int RotationType = 0;
        float Amplitude = 1.0f;
    } m_DomainWarp;

    struct DomainWarpFractalData
    {
        int Type = 0;
        int Octaves = 3;
        float Lacunarity = 2.0f;
        float Gain = 0.5f;
    } m_DomainWarpFractal;
};

#endif