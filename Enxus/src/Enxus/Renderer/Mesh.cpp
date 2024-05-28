#include "pch.h"
#include "Mesh.h"
#include <iostream>

namespace Enxus
{

    Mesh::Mesh(std::vector<VertexData> vertices,
               std::vector<TextureMesh2DData> textures) : m_Vertices(vertices), m_MeshTexturesData(textures)
    {
        CreateBuffers();
        CreateTextures();
    }

    Mesh::Mesh(
        std::vector<VertexData> vertices,
        std::vector<uint32_t> indices,
        std::vector<TextureMesh2DData> textures)
        : m_Vertices(vertices), m_Indices(indices), m_MeshTexturesData(textures)
    {
        CreateBuffers();
        CreateTextures();
    }

    Mesh::Mesh(
        std::vector<VertexData> vertices,
        std::vector<uint32_t> indices,
        std::vector<Ref<TextureMesh2D>> texturesRef)
        : m_Vertices(vertices), m_Indices(indices), m_Textures(texturesRef)
    {
        CreateBuffers();
    }

    Mesh::~Mesh()
    {
        m_Textures.clear();
        // smart pointers delete it selfs
    }

    void Mesh::CreateBuffers()
    {
        m_VertexArrayObject = CreateRef<VertexArray>();

        /*
        VERTEX DEFAULT LAYOUT:
        - Position (3 floats)  x, y, z
        - TexCoords (2 floats) s, t
        - Normal (3 floats)    x, y, z
        */
        BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float2, "aTexCoord"},
            {ShaderDataType::Float3, "aNormal"},
        };

        Ref<VertexBuffer> vertexBuffer = CreateRef<VertexBuffer>(&m_Vertices[0], (uint32_t)m_Vertices.size() * sizeof(VertexData));
        vertexBuffer->SetLayout(layout);
        m_VertexArrayObject->AddVertexBuffer(vertexBuffer);

        if (m_Indices.size() > 0)
        {
            Ref<IndexBuffer> indexBuffer = CreateRef<IndexBuffer>(&m_Indices[0], (uint32_t)m_Indices.size());
            m_VertexArrayObject->SetIndexBuffer(indexBuffer);
        }
    }

    void Mesh::CreateTextures()
    {
        for (auto &[path, type] : m_MeshTexturesData)
        {
            m_Textures.emplace_back(CreateRef<TextureMesh2D>(path, type));
        }
    }
}