#include "ResourceManager.h"

struct ResourceManagerData
{
    ResourcesPath Paths;

    Enxus::Ref<Enxus::SkyBox> SkyBox;
    Enxus::Ref<Enxus::Shader> SkyBoxShader;

    std::vector<Enxus::Ref<Enxus::Model>> ModelsList;
    std::vector<Enxus::Ref<Enxus::TextureMesh2D>> TerrainTexturesList;
};

static ResourceManagerData s_Data;

void ResourceManager::Init()
{

    //----------------- INIT ASSETS PATHS -------------------//

    {
        s_Data.Paths.Images = s_Data.Paths.Base + "images/";
        s_Data.Paths.Models = s_Data.Paths.Base + "models/";
        s_Data.Paths.Shaders = s_Data.Paths.Base + "shaders/";
    }

    //----------------- INIT SKYBOX -------------------//
    {
        s_Data.SkyBox = Enxus::CreateRef<Enxus::SkyBox>();
        s_Data.SkyBox->SetCubeMapFaces(
            {s_Data.Paths.Images + "skybox/right.tga",
             s_Data.Paths.Images + "skybox/left.tga",
             s_Data.Paths.Images + "skybox/top.tga",
             s_Data.Paths.Images + "skybox/bottom.tga",
             s_Data.Paths.Images + "skybox/back.tga",
             s_Data.Paths.Images + "skybox/front.tga"});

        s_Data.SkyBoxShader = Enxus::CreateRef<Enxus::Shader>(
            s_Data.Paths.Shaders + "skybox/skybox.vert",
            s_Data.Paths.Shaders + "skybox/skybox.frag");
    }
    //----------------- TEXTURES -------------------//
    {
        static const std::array<std::string, 7> texturesPaths{
            s_Data.Paths.Images + "materials-debug/water.png",
            s_Data.Paths.Images + "materials-debug/grass.png",
            s_Data.Paths.Images + "materials-debug/rocks1.png",
            s_Data.Paths.Images + "materials-debug/rocks2.png",
            s_Data.Paths.Images + "materials-debug/sandy-grass.png",
            s_Data.Paths.Images + "materials-debug/stony-ground.png",
            s_Data.Paths.Images + "materials-debug/snow.png",
        };

        for (uint32_t i = 0; i < texturesPaths.size(); i++)
        {
            s_Data.TerrainTexturesList.emplace_back(Enxus::CreateRef<Enxus::TextureMesh2D>(texturesPaths[i], Enxus::TextureType::DIFFUSE));
        }
    }

    //----------------- MODELS -------------------//
    {
        // For now must be in the same order as the ImGui enum array in ModelPlacementPanel
        static const std::array<std::string, 7> modelsPaths{
            s_Data.Paths.Models + "box/box.obj",
            s_Data.Paths.Models + "ignore/trees/BirchTree_1.obj",
            s_Data.Paths.Models + "ignore/trees/DeadTree_2.obj",
            s_Data.Paths.Models + "ignore/trees/MapleTree_1.obj",
            s_Data.Paths.Models + "ignore/trees/NormalTree_2.obj",
            s_Data.Paths.Models + "ignore/trees/PalmTree_2.obj",
            s_Data.Paths.Models + "ignore/trees/PineTree_1.obj",
        };

        for (uint32_t i = 0; i < modelsPaths.size(); i++)
        {
            s_Data.ModelsList.emplace_back(Enxus::CreateRef<Enxus::Model>(modelsPaths[i]));
        }
    }
}
void ResourceManager::ShutDown()
{
}
const std::vector<Enxus::Ref<Enxus::Model>> &ResourceManager::GetModelsList()
{
    return s_Data.ModelsList;
}

const std::vector<Enxus::Ref<Enxus::TextureMesh2D>> &ResourceManager::GetTexturesList()
{
    return s_Data.TerrainTexturesList;
}

const Enxus::Ref<Enxus::SkyBox> &ResourceManager::GetSkyBox()
{
    return s_Data.SkyBox;
}

const Enxus::Ref<Enxus::Shader> &ResourceManager::GetSkyBoxShader()
{
    return s_Data.SkyBoxShader;
}

const ResourcesPath &ResourceManager::GetResourcesPath()
{
    return s_Data.Paths;
}