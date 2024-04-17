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
}

void ExampleLayer::OnEvent(Enxus::Event &event)
{
    (void)event;
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