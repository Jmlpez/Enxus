#include "pch/pch.h"
#include "SandboxApp.h"
#include "ExampleLayer.h"

Sandbox::Sandbox()
{
    Enxus::Layer *example = new ExampleLayer();
    PushLayer(example);
}

Sandbox::~Sandbox()
{
}

Enxus::Application *Enxus::CreateApplication()
{
    return new Sandbox();
}