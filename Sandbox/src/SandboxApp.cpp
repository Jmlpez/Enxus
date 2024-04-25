#include "pch.h"
#include "SandboxApp.h"
#include "ExampleLayer.h"
#include "HelloBoxLayer.h"
#include "Editor.h"
#include "TestMenuLayer.h"

Sandbox::Sandbox()
{

    // Enxus::Layer *example = new ExampleLayer();

    // Enxus::Layer *example = new HelloBoxLayer();
    // Enxus::Layer *editor = new Editor();
    Enxus::Layer *testMenu = new TestMenuLayer();
    PushLayer(testMenu);
}

Sandbox::~Sandbox()
{
}

Enxus::Application *Enxus::CreateApplication()
{
    return new Sandbox();
}