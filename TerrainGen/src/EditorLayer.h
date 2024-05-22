#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Enxus.h"

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


    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
    // Framebuffers
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
};
#endif