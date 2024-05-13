#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Enxus.h"
#include "Grid.h"
#include "HeightMapTerrain.h"
#include "TerrainMesh.h"
#include "NoiseMapGenerator.h"
#include "NoiseEditorPanel.h"

class EditorLayer : public Enxus::Layer
{
public:
    EditorLayer();
    ~EditorLayer();

    void OnAttach() override;
    void OnUpdate(Enxus::Timestep ts) override;
    void OnEvent(Enxus::Event &event) override;
    void OnImGuiRender() override;

private:
    void HandleViewportResize();
    void TerrainMenuUI();

private:
    // Editor viewport options
    glm::vec2 m_ViewportSize;
    bool m_IsViewportFocused = false;
    bool m_IsViewportHovered = false;

    // Scene options
    bool m_IsWireframe = false;

    float m_VertexDistance = 0.05f;
    float m_TerrainElevation = 1.5f;
    float m_TextureScale = 1.0f;

    // the first AnimationCurve Enum elemnent: Linear
    int m_TerrainElevationCurve = (int)AnimationCurve::Linear;

    // Color testing
    // glm::vec3 m_BiomeColor[2];
    // float m_BiomeStartHeight[2] = {0.0f, 0.7f};
    // float m_BiomeBlends[2] = {0.5f, 0.5f};

    uint32_t m_TerrainWidth;
    uint32_t m_TerrainHeight;

    glm::vec3 m_LightDirection;

    int m_NumOfBiomeLayers = 1;

    struct BiomeLayersData
    {
        Enxus::Ref<Enxus::TextureMesh2D> Texture;
        glm::vec3 Color = glm::vec3(1.0f);
        float ColorStrenght = 0.5f;
        float StartHeight = 0.0f;
        float BlendStrenght = 0.5f;
        float TextureScale = 1.0f;
    };

    std::array<BiomeLayersData, 8> m_BiomeLayers;

    // Terrain Mesh
    Enxus::Scope<TerrainMesh> m_TerrainMesh;

    // SkyBox
    Enxus::Ref<Enxus::SkyBox> m_SkyBox;
    Enxus::Ref<Enxus::Shader> m_SkyBoxShader;

    // Panels
    Enxus::Scope<NoiseEditorPanel> m_NoiseEditorPanel;
    // Models
    Enxus::Ref<Enxus::Model> m_Box;
    // Shaders
    Enxus::Ref<Enxus::Shader> m_Shader, m_TerrainShader;
    // Camera
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
    // Framebuffers
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
};
#endif