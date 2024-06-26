#ifndef MODEL_H
#define MODEL_H

#include "Base.h"
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

        const std::vector<Ref<Mesh>> &GetMeshes() const { return m_Meshes; }

    private:
        void LoadModel(const std::string &path);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Ref<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Ref<TextureMesh2D>> LoadMaterialTextures(aiMaterial *mat, aiTextureType type);

        std::vector<VertexData> ProcessVertices(aiMesh *mesh) const;
        std::vector<uint32_t> ProcessIndices(aiMesh *mesh) const;
        std::vector<Ref<TextureMesh2D>> ProcessTextures(aiMesh *mesh, const aiScene *scene);

        static TextureType GetTextureType(aiTextureType type);

    private:
        std::vector<Ref<Mesh>> m_Meshes;
        std::string m_ModelPath, m_ModelDirPath;

        // an optimization to avoid load some previously loaded texture
        std::vector<Ref<TextureMesh2D>> m_LoadedTextures;
    };
}
#endif