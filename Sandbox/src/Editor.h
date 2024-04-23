#ifndef EDITOR_H
#define EDITOR_H

#include "Enxus.h"

class Editor : public Enxus::Layer
{
public:
    Editor();
    ~Editor();

    void OnUpdate(Enxus::Timestep ts) override;
    void OnEvent(Enxus::Event &event) override;
    void OnImGuiRender() override;

private:
    glm::vec2 m_ViewportSize;
    bool m_IsViewportFocused = false;
    bool m_IsViewportHovered = false;

    Enxus::Ref<Enxus::Model> m_Box;
    Enxus::Ref<Enxus::Shader> m_Shader;
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
};
#endif