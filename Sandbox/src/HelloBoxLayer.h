#ifndef HELLO_BOX_LAYER_H
#define HELLO_BOX_LAYER_H

#include "Layer.h"
#include "Timestep.h"
#include "Mesh.h"
#include "Shader.h"
#include "FreeCameraController.h"
#include "Renderer.h"

class HelloBoxLayer : public Enxus::Layer
{
public:
    HelloBoxLayer();
    ~HelloBoxLayer();

    void OnUpdate(Enxus::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Enxus::Event &event) override;

private:
    Enxus::Ref<Enxus::Mesh> m_Plane;
    Enxus::Ref<Enxus::Shader> m_Shader;
    Enxus::Scope<Enxus::FreeCameraController> m_CameraController;
};
#endif