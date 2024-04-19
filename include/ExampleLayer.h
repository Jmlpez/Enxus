#ifndef EXAMPLE_LAYER
#define EXAMPLE_LAYER

#include "Layer.h"

class ExampleLayer : public Enxus::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer();

    void OnUpdate() override;
    void OnImGuiRender() override;

    void OnEvent(Enxus::Event &event) override;

private:
    float colors[4] = {0.1f, 0.2f, 0.3f, 1.0f};
};
#endif