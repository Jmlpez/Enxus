#include "pch/pch.h"
#include "SandboxApp.h"

ExampleLayer::ExampleLayer() : Layer("Example")
{
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnUpdate()
{

    std::cout << "ExampleLayer::Update" << std::endl;
}

void ExampleLayer::OnEvent(Enxus::Event &event)
{
    std::cout << "Recieved Event on ExampleLayer: " << event;
}
Sandbox::Sandbox()
{
    PushLayer(new ExampleLayer());
}

Sandbox::~Sandbox()
{
}

Enxus::Application *Enxus::CreateApplication()
{
    return new Sandbox();
}