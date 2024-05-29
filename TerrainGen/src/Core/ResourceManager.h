#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Enxus.h"

struct ResourcesPath
{
    const std::string Base = "TerrainGen/assets/";
    std::string Images;
    std::string Models;
    std::string Shaders;
};

class ResourceManager
{
public:
    static void Init();
    static void ShutDown();

    static const std::vector<Enxus::Ref<Enxus::Model>> &GetModelsList();
    static const std::vector<Enxus::Ref<Enxus::TextureMesh2D>> &GetTexturesList();

    static const Enxus::Ref<Enxus::SkyBox> &GetSkyBox();
    static const Enxus::Ref<Enxus::Shader> &GetSkyBoxShader();

    static const ResourcesPath &GetResourcesPath();

private:
};

#endif