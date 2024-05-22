#ifndef NOISE_EDITOR_PANEL_H
#define NOISE_EDITOR_PANEL_H

#include "FastNoiseLite.h"
#include "Enxus.h"

struct NoiseEditorPanelProps
{
    // this is the size of the generated texture using the noise
    const uint32_t NoiseWidth = 241;
    const uint32_t NoiseHeight = 241;

    FastNoiseLite Fnl;
    FastNoiseLite FnlWarp;

    // To check when update the texture image
    bool NoiseUpdateFlag = true;

    // will be exported to be used in the editor
    //(if not, I would had used a raw array)
    std::vector<float> NoiseMapArray;

    struct FalloffMapData
    {
        bool IsActivated = false;
        float Beta = 2.2f;

        // same width and height that the noise map texture
        Enxus::Scope<Enxus::Texture2D> Texture;

    } FalloffMap;

    struct NoiseTexturePreview
    {

        bool IsAutoSize = true;
        float ColorTexMin = -1.0f;
        float ColorTexMax = 1.0f;
        // this is the display size(a.k.a the size in ImGui window)
        int ImGuiWidth;
        int ImGuiHeight;
        Enxus::Scope<Enxus::Texture2D> Texture;

    } NoisePreviewData;

    struct GeneralNoiseData
    {
        bool IsInverted = false;
        int NoiseType = 0;
        int Seed = 1337;

        float Frequency = 0.01f;
        float OffsetX = 0.0f;
        float OffsetY = 0.0f;

    } GeneralNoise;

    struct FractalNoiseData
    {
        int Type = 0;
        int Octaves = 3;
        float Lacunarity = 2.0f;
        float Gain = 0.5f;
        float WeightedStrength = 0.0f;
        float PingPongStrength = 2.0f;
    } FractalNoise;

    struct CellularNoiseData
    {
        int Type = 1;
        int ReturnType = 1;
        float Jitter = 1.0f;
    } CellularNoise;

    struct DomainWarpData
    {
        int Seed = 1337;
        float Frequency = 0.01f;
        int Type = 0;
        int RotationType = 0;
        float Amplitude = 1.0f;
    } DomainWarp;

    struct DomainWarpFractalData
    {
        int Type = 0;
        int Octaves = 3;
        float Lacunarity = 2.0f;
        float Gain = 0.5f;
    } DomainWarpFractal;
};

class NoiseEditorPanel
{
public:
    static void Init();
    static void ShutDown();

    static void OnImGuiRender();
    static void SetNoiseWidth(uint32_t width);
    static void SetNoiseHeight(uint32_t height);

    static bool HasUpdated();
    static uint32_t GetNoiseWidth();
    static uint32_t GetNoiseHeight();
    static const std::vector<float> &GetNoiseMap();

private:
    static void UpdateNoiseMap(bool newMap);
};

#endif