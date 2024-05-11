#include "TerrainMesh.h"

const uint32_t TerrainMesh::s_MaxTerrainSize = 1000;

TerrainMesh::TerrainMesh()
{
    m_VertexArrayObject = Enxus::CreateRef<Enxus::VertexArray>();
    m_GrassTexture = Enxus::CreateRef<Enxus::TextureMesh2D>("TerrainGen/assets/images/container.png",
                                                            Enxus::TextureType::DIFFUSE);
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

    m_GrassTexture = Enxus::CreateRef<Enxus::TextureMesh2D>("TerrainGen/assets/images/grass-albedo.png",
                                                            Enxus::TextureType::DIFFUSE);
    m_SnowTexture = Enxus::CreateRef<Enxus::TextureMesh2D>("TerrainGen/assets/images/snow-albedo.png",
                                                           Enxus::TextureType::DIFFUSE);

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

            m_Vertices[vertexIndex].Position.x = (-(float)m_Width / 2.0f + i) * m_VertexDistance;  // to center int x-axis
            m_Vertices[vertexIndex].Position.z = (-(float)m_Height / 2.0f + j) * m_VertexDistance; // to center int z-axis
        }
    }
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(TerrainVertex));
}

void TerrainMesh::SetElevation(float elevation)
{
    if (m_Elevation == elevation)
        return;

    m_Elevation = elevation;
    if (m_NoiseMap.empty())
        return;

    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            uint32_t vertexIndex = i * m_Width + j;
            float newYPos = m_NoiseMap[vertexIndex] * m_Elevation;
            m_Vertices[vertexIndex].Position.y = newYPos;
        }
    }
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(TerrainVertex));
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
            float newYPos = m_NoiseMap[vertexIndex] * m_Elevation;
            m_Vertices[vertexIndex].Position.y = newYPos;
        }
    }
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(TerrainVertex));
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
    m_VertexBufferObject = Enxus::CreateRef<Enxus::VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(TerrainVertex));

    /*
        Terrain Vertex Layout
        x, y, z --> position
        x, y --> texture coordinates
    */
    Enxus::VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);
    layout.Push(2, GL_FLOAT);

    m_VertexArrayObject->AddBuffer(m_VertexBufferObject, layout);

    std::vector<unsigned int> indices = std::move(CreateIndices());
    m_IndexBufferObject = Enxus::CreateRef<Enxus::IndexBuffer>(&indices[0], indices.size());
}

void TerrainMesh::CreateVertices()
{
    std::vector<TerrainVertex> vertices;
    vertices.reserve(m_Width * m_Height);

    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            glm::vec3 position;
            position.x = (-(float)m_Width / 2.0f + i) * m_VertexDistance;  // to center int x-axis
            position.y = 0.0f;                                             // flat plane
            position.z = (-(float)m_Height / 2.0f + j) * m_VertexDistance; // to center int z-axis

            glm::vec2 texCoord;
            texCoord.x = (1.0f / (float)m_Width) * j;
            texCoord.y = (1.0f / (float)m_Height) * i;

            vertices.emplace_back(
                TerrainVertex(
                    position,
                    texCoord));
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