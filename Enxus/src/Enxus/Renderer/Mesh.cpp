#include "pch.h"
#include "Mesh.h"
#include <iostream>

namespace Enxus
{

    Mesh::Mesh(std::vector<VertexData> vertices,
               std::vector<TextureData2D> textures) : m_Vertices(vertices), m_MeshTextures(textures)
    {
        CreateBuffers();
        CreateTextures();
    }

    Mesh::Mesh(
        std::vector<VertexData> vertices,
        std::vector<unsigned int> indices,
        std::vector<TextureData2D> textures)
        : m_Vertices(vertices), m_Indices(indices), m_MeshTextures(textures)
    {
        CreateBuffers();
        CreateTextures();
    }

    Mesh::Mesh(
        std::vector<VertexData> vertices,
        std::vector<unsigned int> indices,
        std::vector<Ref<Texture2D>> texturesRef)
        : m_Vertices(vertices), m_Indices(indices), m_Textures(texturesRef)
    {
        CreateBuffers();
    }

    Mesh::~Mesh()
    {
        // smart pointers delete it selfs
    }

    void Mesh::CreateBuffers()
    {
        m_VertexArrayObject = CreateRef<VertexArray>();
        m_VertexBufferObject = CreateRef<VertexBuffer>(&m_Vertices[0], (unsigned int)m_Vertices.size() * sizeof(VertexData));

        if (m_Indices.size() > 0) // if the mesh has indices
            m_IndexBufferObject = CreateRef<IndexBuffer>(&m_Indices[0], (unsigned int)m_Indices.size());

        /*
        VERTEX DEFAULT LAYOUT:
        - Position (3 floats)  x, y, z
        - TexCoords (2 floats) s, t
        - Normal (3 floats)    x, y, z
        */
        VertexBufferLayout layout;
        layout.Push(3, GL_FLOAT);
        layout.Push(2, GL_FLOAT);
        layout.Push(3, GL_FLOAT);

        m_VertexArrayObject->AddBuffer(*m_VertexBufferObject, layout);
        /*
        IGNORE:
        Assuming a struct call MeshTexture defined as:
        struct MeshTexture{
            std::string path;
            TEXTURE_TYPE
        }
        */
    }

    void Mesh::CreateTextures()
    {
        for (auto &[path, type] : m_MeshTextures)
        {
            m_Textures.push_back(CreateRef<Texture2D>(path, type));
        }
    }
}