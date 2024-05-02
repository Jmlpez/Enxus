#ifndef HELLO_BOX_LAYER_H
#define HELLO_BOX_LAYER_H

#include "Enxus.h"

class HelloBoxLayer : public Enxus::Layer
{
public:
    HelloBoxLayer();
    ~HelloBoxLayer();

    void OnUpdate(Enxus::Timestep ts) override;
    void OnEvent(Enxus::Event &event) override;
    void OnImGuiRender() override;

private:
    Enxus::Ref<Enxus::Mesh> m_Plane;
    Enxus::Ref<Enxus::TextureMesh2D> m_ExampleTexture;
    Enxus::Ref<Enxus::Framebuffer> m_Framebuffer;
    Enxus::Ref<Enxus::Shader> m_Shader;
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
};
#endif