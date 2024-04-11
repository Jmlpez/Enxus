#ifndef MESH_H
#define MESH_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "glm/glm.hpp"
#include <vector>

struct MeshVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MeshTexture
{
    std::string Path;
    TEXTURE_TYPE Type;
};

class Mesh
{
private:
    std::shared_ptr<VertexArray> m_VertexArrayObject;
    std::shared_ptr<VertexBuffer> m_VertexBufferObject;
    std::shared_ptr<IndexBuffer> m_IndexBufferObject;

    std::vector<MeshVertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<MeshTexture> m_MeshTextures;

    std::vector<std::shared_ptr<Texture2D>> m_Textures;

public:
    Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    ~Mesh();

    void SetUpMesh();

    const std::vector<MeshVertex> &GetVertices() const { return m_Vertices; }
    const std::vector<unsigned int> &GetIndices() const { return m_Indices; }
    const std::vector<std::shared_ptr<Texture2D>> &GetTextures() const { return m_Textures; }

    std::shared_ptr<VertexArray> GetVAO() const { return m_VertexArrayObject; }
    std::shared_ptr<VertexBuffer> GetVBO() const { return m_VertexBufferObject; }
    std::shared_ptr<IndexBuffer> GetIBO() const { return m_IndexBufferObject; }
};

#endif