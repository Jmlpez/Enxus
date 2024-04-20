#include "pch.h"
#include "SandboxApp.h"
#include "ExampleLayer.h"
#include "HelloBoxLayer.h"

Sandbox::Sandbox()
{

    // Enxus::Layer *example = new ExampleLayer();

    Enxus::Layer *example = new HelloBoxLayer();
    PushLayer(example);
}

Sandbox::~Sandbox()
{
}

Enxus::Application *Enxus::CreateApplication()
{
    return new Sandbox();
}