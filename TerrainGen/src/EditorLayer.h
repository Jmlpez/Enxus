#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Enxus.h"
#include "Grid.h"
#include "HeightMapTerrain.h"

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

private:
    glm::vec2 m_ViewportSize;
    bool m_IsViewportFocused = false;
    bool m_IsViewportHovered = false;

    bool m_ShowGridFloor = false;
    bool m_IsWireframe = false;

    Enxus::Ref<Enxus::Model> m_Box;
    Enxus::Scope<Grid> m_GridFloor;
    Enxus::Scope<HeightMapTerrain> m_Terrain;

    Enxus::Ref<Enxus::Shader> m_Shader, m_GridShader, m_TerrainShader;
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
};
#endif