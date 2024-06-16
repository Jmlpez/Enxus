#ifndef EROSION_PANEL_H
#define EROSION_PANEL_H

#include "Enxus.h"
#include "Erosion.h"

struct ErosionPanelProps
{
    bool IsErosionOn;
    bool RevertErosion;
    int Iterations;
    Erosion::ErosionProps ErosionConstants;
};

class ErosionPanel
{
public:
    static void Init();
    static void ShutDown();

    static const ErosionPanelProps &GetPanelProps();

    static void OnImGuiRender();
};

#endif