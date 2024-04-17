#include "pch/pch.h"
#include "ImGuiLayer.h"
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
    std::cout << "pepe";
    PushLayer(new ExampleLayer());
    PushOverlay(new Enxus::ImGuiLayer());
}

Sandbox::~Sandbox()
{
}

Enxus::Application *Enxus::CreateApplication()
{
    return new Sandbox();
}