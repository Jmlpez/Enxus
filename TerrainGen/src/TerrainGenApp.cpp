#include "pch.h"
#include "TerrainGenApp.h"
#include "EditorLayer.h"

TerrainGenApp::TerrainGenApp()
{
    Enxus::Layer *layer = new EditorLayer();
    PushLayer(layer);
}

TerrainGenApp::~TerrainGenApp()
{
}

Enxus::Application *Enxus::CreateApplication()
{
    return new TerrainGenApp();
}