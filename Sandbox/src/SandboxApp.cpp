#include "pch.h"
#include "SandboxApp.h"
#include "TestMenuLayer.h"

Sandbox::Sandbox()
{
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