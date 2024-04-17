#ifndef SANDBOX_APP
#define SANDBOX_APP

#include "Application.h"

class ExampleLayer : public Enxus::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer();

    void OnUpdate() override;

    void OnEvent(Enxus::Event &event) override;
};

class Sandbox : public Enxus::Application
{
public:
    Sandbox();
    ~Sandbox();
};

#endif