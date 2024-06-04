#ifndef SCENE_EDITOR_PANEL_H
#define SCENE_EDITOR_PANEL_H

#include "Enxus.h"

struct SceneCompositionPanelProps
{
    bool IsWireframe;
    bool UseShadows;
    glm::vec3 LightPosition;

    struct OrthoProjData
    {
        bool IsDebugging = false;
        float NearPlane;
        float FarPlane;
        float WidthLimit;
        float HeightLimit;
    } OrthoProj;
};

class SceneCompositionPanel
{
public:
    static void Init();
    static void ShutDown();

    static const SceneCompositionPanelProps &GetPanelProps();

    static void OnImGuiRender();
};

#endif