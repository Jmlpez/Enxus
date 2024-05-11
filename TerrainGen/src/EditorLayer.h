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

    float m_HeightScale = 1.5f;
    float m_VertexDistance = 0.05f;

    uint32_t m_TerrainWidth;
    uint32_t m_TerrainHeight;

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