#ifndef SCENE_EDITOR_PANEL_H
#define SCENE_EDITOR_PANEL_H

#include "Enxus.h"

struct SceneCompositionPanelProps
{
    bool IsWireframe;
    glm::vec3 LightDirection;
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