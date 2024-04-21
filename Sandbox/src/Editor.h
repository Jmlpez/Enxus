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
    Enxus::Ref<Enxus::Model> m_Box;
    Enxus::Ref<Enxus::Shader> m_Shader;
    Enxus::Scope<Enxus::Framebuffer> m_Framebuffer;
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
};
#endif