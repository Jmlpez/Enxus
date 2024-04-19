#include "pch/pch.h"
#include "ExampleLayer.h"
#include "ImGuiLayer.h"
#include "Input.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "SandboxApp.h"

ExampleLayer::ExampleLayer() : Layer("Example")
{
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnImGuiRender()
{
}

void ExampleLayer::OnUpdate()
{

    if (Enxus::Input::IsKeyPressed(Enxus::Key::Escape))
        Enxus::Application::Get().Close();
}

void ExampleLayer::OnEvent(Enxus::Event &event)
{
}
