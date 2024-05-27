#ifndef MODEL_PLACEMENT_PANEL_H
#define MODEL_PLACEMENT_PANEL_H

#include "Enxus.h"

struct ModelPlacementPanelProps
{
    const int MaxNumOfModels = 8;
    const int MaxAmount = 5000;
    struct ModelPlacementData
    {
        float Radius = 8.0f;
        float OffsetHeight = 0.0f;
        float Scale = 0.1f;
        int Amount = 250;
        int ModelIndex = 0;
        float HeightRangeBegin = 0.0f, HeightRangeEnd = 1.0f;
    };

    int NumOfModels = 0;
    std::array<ModelPlacementData, 8> ModelsData;
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