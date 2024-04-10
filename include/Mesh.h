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

class Mesh
{
private:
    std::shared_ptr<VertexArray> m_VertexArrayObject;
    std::shared_ptr<VertexBuffer> m_VertexBufferObject;
    std::shared_ptr<IndexBuffer> m_IndexBufferObject;
    std::vector<MeshVertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<std::shared_ptr<Texture2D>> m_Textures;
    // std::vector<Texture2D *> m_TexturesPointer;

public:
    // Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> &textures);
    Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> &textures);
    ~Mesh();

    void SetUpMesh();

    const std::vector<MeshVertex> &GetVertices() const { return m_Vertices; }
    const std::vector<unsigned int> &GetIndices() const { return m_Indices; }
    const std::vector<std::shared_ptr<Texture2D>> &GetTextures() const { return m_Textures; }
    // const std::vector<Texture2D *> &GetTexturesPointer() const { return m_TexturesPointer; }

    const VertexArray *GetVAO() const { return m_VertexArrayObject; }
    const VertexBuffer *GetVBO() const { return m_VertexBufferObject; }
    const IndexBuffer *GetIBO() const { return m_IndexBufferObject; }
};

#endif