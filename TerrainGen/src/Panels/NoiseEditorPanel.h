#ifndef NOISE_EDITOR_PANEL_H
#define NOISE_EDITOR_PANEL_H

#include "Enxus.h"
#include "FastNoiseLite.h"
#include "imgui/imgui.h"

class NoiseEditorPanel
{
public:
    NoiseEditorPanel();
    ~NoiseEditorPanel(){};

    void OnImGuiRender();

    void SetWidth(uint32_t width);
    void SetHeight(uint32_t height);

    inline bool HasUpdated() { return m_NoiseUpdateFlag; }
    inline uint32_t GetWidth() { return (uint32_t)m_GeneralNoise.Width; }
    inline uint32_t GetHeight() { return (uint32_t)m_GeneralNoise.Height; }
    inline const std::vector<float> &GetNoiseMap() const { return m_NoiseMapArray; }

private:
    void UpdateNoiseMap(bool newPreview);

private:
    // this is the size of the generated texture using the noise
    static const int s_TextureGeneratedWidth;
    static const int s_TextureGeneratedHeight;

    FastNoiseLite m_Fnl;
    FastNoiseLite m_FnlWarp;

    // To check when update the texture image
    bool m_NoiseUpdateFlag = true;

    struct FalloffMapData
    {
        bool IsActivated = false;
        float Beta = 2.2f;

        // same width and height that the noise map texture
        Enxus::Scope<Enxus::Texture2D> Texture;

    } m_FalloffMap;

    struct NoiseTexturePreview
    {

        bool IsAutoSize = true;
        float ColorTexMin = -1.0f;
        float ColorTexMax = 1.0f;
        // this is the display size(a.k.a the size in ImGui window)
        int ImGuiWidth;
        int ImGuiHeight;
        Enxus::Scope<Enxus::Texture2D> Texture;

    } m_NoisePreviewData;

    // its going to be used only in this class

    // will be exported to be used in the editor
    //(if not, i would had been used a raw array)
    std::vector<float> m_NoiseMapArray;

    struct GeneralNoiseData
    {
        int Width;
        int Height;
        int NoiseType = 0;
        int Seed = 1337;

        float Frequency = 0.01f;
        float OffsetX = 0.0f;
        float OffsetY = 0.0f;

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