#include "Model.h"
#include <iostream>

Model::Model(const std::string &path) : m_FullPath(path)
{
    LoadModel(path);
}

Model::~Model()
{
}

void Model::LoadModel(const std::string &path)
{
    Assimp::Importer importer;

    auto importerStepsFlags = aiProcess_Triangulate | aiProcess_FlipUVs;
    /*
    Other usefull options
    aiProcess_GenNormals:
    creates normal vectors for each vertex if the model doesn’tcontain normal vectors.

    aiProcess_SplitLargeMeshes:
    splits large meshes into smaller sub-meshes which
    is useful if your rendering has a maximum number of vertices allowed
    and can only process smaller meshes.

    aiProcess_OptimizeMeshes:
    does the reverse by trying to join several meshes into
    one larger mesh, reducing drawing calls for optimization.
    */

    const aiScene *scene = importer.ReadFile(path, importerStepsFlags);

    // If there is a problem loading the scene or the model
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "[ASSIMP ERROR] :" << importer.GetErrorString() << std::endl;
        return;
    }
    m_DirPath = m_FullPath.substr(0, m_FullPath.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    // load all meshes of the given node (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    // recursively process the node childrens in a DFS-fashion (if any)
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}
std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<MeshVertex> vertices = ProcessVertices(mesh);
    std::vector<unsigned int> indices = ProcessIndices(mesh);
    std::vector<MeshTexture> textures = ProcessTextures(mesh, scene);

    return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<MeshVertex> Model::ProcessVertices(aiMesh *mesh)
{
    std::vector<MeshVertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        MeshVertex vertex;
        glm::vec3 tempVec;

        //----------------- POSITION -------------------//
        tempVec.x = mesh->mVertices[i].x;
        tempVec.y = mesh->mVertices[i].y;
        tempVec.z = mesh->mVertices[i].z;
        vertex.Position = tempVec;
        //----------------- NORMALS -------------------//
        tempVec.x = mesh->mNormals[i].x;
        tempVec.y = mesh->mNormals[i].y;
        tempVec.z = mesh->mNormals[i].z;
        vertex.Normal = tempVec;
        //----------------- TEXTURE COORDINATES -------------------//
        // the first set of textures (assimp hold up to 8)
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 coords;
            coords.x = mesh->mTextureCoords[0][i].x;
            coords.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = coords;
        }
        else
        { // does not has textures...
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    return vertices;
}

std::vector<unsigned int> Model::ProcessIndices(aiMesh *mesh)
{

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace &face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    return indices;
}

std::vector<MeshTexture> Model::ProcessTextures(aiMesh *mesh, const aiScene *scene)
{
    std::vector<MeshTexture> textures;
    if (!(mesh->mMaterialIndex >= 0))
        return textures;

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<MeshTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<MeshTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
}

std::vector<MeshTexture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString texPath;
        mat->GetTexture(type, i, &texPath);
        bool isTextureLoaded = false;
        for (auto tex : m_LoadedTextures)
        {
            if (std::strcmp(tex.Path.c_str(), texPath.C_Str()) == 0)
            {
                textures.push_back(tex);
                isTextureLoaded = true;
                break;
            }
        }
        if (isTextureLoaded)
            continue;

        MeshTexture texture;
        texture.Type = GetTextureType(type);
        texture.Path = std::string(texPath.C_Str());
        textures.push_back(texture);
        m_LoadedTextures.push_back(texture);
    }
    return textures;
}

TEXTURE_TYPE Model::GetTextureType(aiTextureType type)
{
    switch (type)
    {
    case aiTextureType_DIFFUSE:
        return TEXTURE_TYPE::DIFFUSE;
    case aiTextureType_SPECULAR:
        return TEXTURE_TYPE::SPECULAR;
    case aiTextureType_NORMALS:
        return TEXTURE_TYPE::NORMAL;
    default:
        break;
    }
    std::cout << "[ASSIMP ERROR]: Unknow texture type" << std::endl;
    ASSERT(false);

    // return ()0;
}