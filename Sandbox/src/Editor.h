#ifndef EDITOR_H
#define EDITOR_H

#include "Enxus.h"
#include "Grid.h"

class Editor : public Enxus::Layer
{
public:
    Editor();
    ~Editor();

    void OnUpdate(Enxus::Timestep ts) override;
    void OnEvent(Enxus::Event &event) override;
    void OnImGuiRender() override;

private:
    void HandleViewportResize();

private:
    glm::vec2 m_ViewportSize;
    bool m_IsViewportFocused = false;
    bool m_IsViewportHovered = false;

    bool m_ShowGridFloor = true;

    Enxus::Ref<Enxus::Model> m_Box;
    Enxus::Scope<Grid> m_GridFloor;

    Enxus::Ref<Enxus::Shader> m_Shader, m_GridShader;
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
};
#endif