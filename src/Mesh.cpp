#include "Mesh.h"

Mesh::Mesh(
    std::vector<MeshVertex> vertices,
    std::vector<unsigned int> indices,
    std::vector<MeshTexture> textures)
    : m_Vertices(vertices), m_Indices(indices), m_MeshTextures(textures)
{
    SetUpMesh();
}

Mesh::~Mesh()
{
    // smart pointers delete it selfs
}

void Mesh::SetUpMesh()
{
    m_VertexArrayObject = std::make_shared<VertexArray>();
    m_VertexBufferObject = std::make_shared<VertexBuffer>(&m_Vertices[0], (unsigned int)m_Vertices.size() * sizeof(MeshVertex));
    m_IndexBufferObject = std::make_shared<IndexBuffer>(&m_Indices[0], (unsigned int)m_Indices.size());

    /*
    VERTEX DEFAULT LAYOUT:
    - Position (3 floats)  x, y, z
    - Normal (3 floats)    x, y, z
    - TexCoords (2 floats) s, t
    */
    VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);
    layout.Push(3, GL_FLOAT);
    layout.Push(2, GL_FLOAT);

    m_VertexArrayObject->AddBuffer(*m_VertexBufferObject, layout);
    /*
    IGNORE:
    Assuming a struct call MeshTexture defined as:
    struct MeshTexture{
        std::string path;
        TEXTURE_TYPE
    }
    */
    for (auto &[path, type] : m_MeshTextures)
    {
        m_Textures.push_back(std::make_shared<Texture2D>(path, type));
    }
}