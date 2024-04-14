#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

namespace Enxus
{

    class Model
    {
    public:
        Model(const std::string &path);
        ~Model();

        std::vector<Ref<Mesh>> &GetMeshes() { return m_Meshes; }

    private:
        std::vector<Ref<Mesh>> m_Meshes;
        std::string m_ModelPath, m_ModelDirPath;

        // an optimization to avoid load some previously loaded texture
        std::vector<Ref<Texture2D>> m_LoadedTextures;

        void LoadModel(const std::string &path);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Ref<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Ref<Texture2D>> LoadMaterialTextures(aiMaterial *mat, aiTextureType type);

        std::vector<VertexData> ProcessVertices(aiMesh *mesh);
        std::vector<unsigned int> ProcessIndices(aiMesh *mesh);
        std::vector<Ref<Texture2D>> ProcessTextures(aiMesh *mesh, const aiScene *scene);

        static TextureType GetTextureType(aiTextureType type);
    };
}
#endif