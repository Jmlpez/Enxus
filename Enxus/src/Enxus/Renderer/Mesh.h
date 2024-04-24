#ifndef MESH_H
#define MESH_H

#include "Base.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "glm/glm.hpp"

namespace Enxus
{

    struct VertexData
    {
        glm::vec3 Position;
        glm::vec2 TexCoords;
        glm::vec3 Normal;
        VertexData(glm::vec3 _position, glm::vec2 _texCoords, glm::vec3 _normal)
            : Position(_position), TexCoords(_texCoords), Normal(_normal)
        {
        }
        VertexData() : Position(0), TexCoords(0), Normal(0) {}
    };

    class Mesh
    {

    public:
        Mesh(std::vector<VertexData> vertices, std::vector<TextureData2D> textures);
        Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<TextureData2D> textures);
        Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture2D>> texturesRef);
        ~Mesh();

        void CreateBuffers();
        void CreateTextures();

        const std::vector<VertexData> &GetVertices() const { return m_Vertices; }
        const std::vector<unsigned int> &GetIndices() const { return m_Indices; }
        const std::vector<Ref<Texture2D>> &GetTextures() const { return m_Textures; }

        Ref<VertexArray> GetVertexArray() const { return m_VertexArrayObject; }
        Ref<VertexBuffer> GetVertexBuffer() const { return m_VertexBufferObject; }
        Ref<IndexBuffer> GetIndexBuffer() const { return m_IndexBufferObject; }

        // smart pointers has the bool operator overloaded
        bool HasIndices() { return (bool)m_IndexBufferObject; }

    private:
        Ref<VertexArray> m_VertexArrayObject;
        Ref<VertexBuffer> m_VertexBufferObject;
        Ref<IndexBuffer> m_IndexBufferObject;

        std::vector<VertexData> m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<TextureData2D> m_MeshTexturesData;

        std::vector<Ref<Texture2D>> m_Textures;
    };
}
#endif