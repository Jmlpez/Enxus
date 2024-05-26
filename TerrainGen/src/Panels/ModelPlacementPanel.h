#ifndef MODEL_PLACEMENT_PANEL_H
#define MODEL_PLACEMENT_PANEL_H

#include "Enxus.h"

struct ModelPlacementPanelProps
{
    float Radius = 25.0f;
    float Scale = 0.01f;
    uint32_t Amount = 1000;
};

class ModelPlacementPanel
{
public:
    static void Init();
    static void ShutDown();

    static const ModelPlacementPanelProps &GetPanelProps();

    static void OnImGuiRender();
};

#endif