#include "TerrainMesh.h"

const uint32_t TerrainMesh::s_MaxTerrainSize = 1000;

TerrainMesh::TerrainMesh()
{
    m_VertexArrayObject = Enxus::CreateRef<Enxus::VertexArray>();
}

TerrainMesh::TerrainMesh(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height)
{
    if (m_Width >= s_MaxTerrainSize || m_Height >= s_MaxTerrainSize)
    {
        std::cout << "[TerrainMesh Warning] Max size allowed is  " << s_MaxTerrainSize << std::endl;
    }
    m_Width = std::min(m_Height, s_MaxTerrainSize);
    m_Height = std::min(m_Width, s_MaxTerrainSize);

    m_VertexArrayObject = Enxus::CreateRef<Enxus::VertexArray>();
    CreateTerrain();
}

TerrainMesh::~TerrainMesh()
{
}

void TerrainMesh::SetVertexDistance(float distance)
{
    if (distance == m_VertexDistance)
        return;
    m_VertexDistance = distance;
    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            uint32_t vertexIndex = i * m_Width + j;

            m_Vertices[vertexIndex].x = (-(float)m_Width / 2.0f + i) * m_VertexDistance;  // to center int x-axis
            m_Vertices[vertexIndex].z = (-(float)m_Height / 2.0f + j) * m_VertexDistance; // to center int z-axis
        }
    }
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3));
}

void TerrainMesh::SetHeightScaleFactor(float heightScale)
{
    if (m_HeightScale == heightScale)
        return;

    m_HeightScale = heightScale;
    if (m_NoiseMap.empty())
        return;

    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            uint32_t vertexIndex = i * m_Width + j;
            float newYPos = m_NoiseMap[vertexIndex] * m_HeightScale;
            m_Vertices[vertexIndex].y = newYPos;
        }
    }
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3));
}

void TerrainMesh::SetNoiseMap(const std::vector<float> &noiseMap)
{
    // Add a loggin system with asserts and macros
    // ASSERT(noiseMap.size() != m_Width * m_Height);
    // asserts are slow so keep it with the macros !
    m_NoiseMap = std::move(noiseMap);
    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            uint32_t vertexIndex = i * m_Width + j;
            float newYPos = m_NoiseMap[vertexIndex] * m_HeightScale;
            m_Vertices[vertexIndex].y = newYPos;
        }
    }
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3));
}

void TerrainMesh::SetWidth(uint32_t width)
{
    m_Width = width;

    m_Vertices.clear();
    //    m_Indices.clear();
    m_VertexBufferObject.reset();
    m_IndexBufferObject.reset();

    CreateTerrain();
}
void TerrainMesh::SetHeight(uint32_t height)
{
    m_Height = height;

    m_Vertices.clear();
    //    m_Indices.clear();
    m_VertexBufferObject.reset();
    m_IndexBufferObject.reset();

    CreateTerrain();
}

void TerrainMesh::CreateTerrain()
{
    CreateVertices();
    m_VertexBufferObject = Enxus::CreateRef<Enxus::VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3));

    /*
        Grid Vertex Layout
        x, y, z --> position
    */
    Enxus::VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);

    m_VertexArrayObject->AddBuffer(m_VertexBufferObject, layout);

    std::vector<unsigned int> indices = std::move(CreateIndices());
    m_IndexBufferObject = Enxus::CreateRef<Enxus::IndexBuffer>(&indices[0], indices.size());
}

void TerrainMesh::CreateVertices()
{
    std::vector<glm::vec3> vertices;
    vertices.reserve(m_Width * m_Height);

    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            glm::vec3 position;
            position.x = (-(float)m_Width / 2.0f + i) * m_VertexDistance;  // to center int x-axis
            position.y = 0.0f;                                             // flat plane
            position.z = (-(float)m_Height / 2.0f + j) * m_VertexDistance; // to center int z-axis
            vertices.emplace_back(position);
        }
    }
    m_Vertices = std::move(vertices);
}

std::vector<unsigned int> TerrainMesh::CreateIndices()
{
    std::vector<unsigned int> indices((m_Height - 1) * (m_Width * 2), 0);

    int index = 0;
    // Triangle strip indices
    for (uint32_t i = 0; i < m_Height - 1; i++)
    {
        int offset = m_Width * i;
        indices[index] = offset;
        indices[index + 1] = offset + m_Width;
        index += 2;
        for (uint32_t j = 1; j < m_Width; j++)
        {
            indices[index] = offset + j;
            indices[index + 1] = offset + j + m_Width;
            index += 2;
        }
    }
    return indices;
}