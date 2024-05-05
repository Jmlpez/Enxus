#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Enxus.h"
#include "Grid.h"
#include "HeightMapTerrain.h"
#include "TerrainMesh.h"
#include "NoiseMapGenerator.h"

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
    void NoiseGeneratorPanelUI();

private:
    glm::vec2 m_ViewportSize;
    bool m_IsViewportFocused = false;
    bool m_IsViewportHovered = false;

    bool m_ShowGridFloor = false;
    bool m_IsWireframe = false;

    // Enxus::Scope<Enxus::Texture2D> m_NoiseTexture;

    Enxus::Scope<TerrainMesh> m_TerrainMesh;

    Enxus::Scope<NoiseMapGenerator> m_NoiseMapGen;

    float m_Frequency = 0.025f, m_Lacuranity = 2.0f;
    float m_Amplitude = 2.0f, m_Persistance = 0.5f;

    Enxus::Ref<Enxus::Model> m_Box;
    Enxus::Scope<Grid> m_GridFloor;
    Enxus::Scope<HeightMapTerrain> m_Terrain;

    Enxus::Ref<Enxus::Shader> m_Shader, m_GridShader, m_TerrainShader;
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
};
#endif