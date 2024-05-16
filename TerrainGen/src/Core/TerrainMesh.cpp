#include "TerrainMesh.h"

const uint32_t TerrainMesh::s_MaxTerrainSize = 500;

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

void TerrainMesh::CreateTerrain()
{
    CreateVertices();
    m_VertexBufferObject = Enxus::CreateRef<Enxus::VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(TerrainVertex));

    /*
        Terrain Vertex Layout
        x, y, z --> position
        x, y --> texture coordinates
        x, y, z --> normal vector
    */
    Enxus::VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);
    layout.Push(2, GL_FLOAT);
    layout.Push(3, GL_FLOAT);

    m_VertexArrayObject->AddBuffer(m_VertexBufferObject, layout);

    m_Indices = std::move(CreateIndices());
    m_IndexBufferObject = Enxus::CreateRef<Enxus::IndexBuffer>(&m_Indices[0], m_Indices.size());
}

void TerrainMesh::CreateVertices()
{
    std::vector<TerrainVertex> vertices;
    vertices.reserve(m_Width * m_Height);

    float topLeftX = (float)m_Width / -2.0f;
    float topLeftZ = (float)m_Height / -2.0f;

    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            // Position
            glm::vec3 position;

            position.x = (topLeftX + j) * m_VertexDistance; // to center int x-axis
            position.y = 0.0f;                              // flat plane
            position.z = (topLeftZ + i) * m_VertexDistance; // to center int z-axis

            // TexCoords
            glm::vec2 texCoord;
            texCoord.x = (float)j / (float)m_Width;
            texCoord.y = (float)i / (float)m_Height;

            // Normal (0 value initially)
            glm::vec3 normal(0.0f, 1.0f, 0.0f);

            vertices.emplace_back(position, texCoord, normal);
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
        for (uint32_t j = 0; j < m_Width; j++)
        {
            indices[index] = offset + j;
            indices[index + 1] = offset + j + m_Width;
            index += 2;
        }
    }
    return indices;
}

void TerrainMesh::CalculateNormals()
{

    // Reset  TerrainVertex normals to 0
    for (auto &vertex : m_Vertices)
        vertex.Normal = glm::vec3(0);

    uint32_t numOfStrips = m_Height - 1;
    for (uint32_t strip = 0; strip < numOfStrips; strip++)
    {
        uint32_t numOfVertexPerStrip = m_Width * 2;
        uint32_t indexOffset = strip * numOfVertexPerStrip;
        bool flipDir = false;
        for (uint32_t i = 2; i < numOfVertexPerStrip; i++)
        {
            unsigned int indexA = m_Indices[indexOffset + i - 2];
            unsigned int indexB = m_Indices[indexOffset + i - 1];
            unsigned int indexC = m_Indices[indexOffset + i];

            glm::vec3 normal = GetNormalFromIndices(indexA, indexB, indexC, flipDir);

            m_Vertices[indexA].Normal += normal;
            m_Vertices[indexB].Normal += normal;
            m_Vertices[indexC].Normal += normal;

            // swap the cross product order in each iteration
            // because of the 2 triangles of a single face
            flipDir = !flipDir;
        }
    }

    // Normalize the TerrainVertex normals
    for (auto &vertex : m_Vertices)
        vertex.Normal = glm::normalize(vertex.Normal);
}

void TerrainMesh::SetHeightElevationCurve(AnimationCurve curve)
{
    m_ElevationCurve = curve;
    CalculateNoiseMap();
}

void TerrainMesh::SetVertexDistance(float distance)
{
    if (distance == m_VertexDistance)
        return;
    m_VertexDistance = distance;

    float topLeftX = (float)m_Width / -2.0f;
    float topLeftZ = (float)m_Height / -2.0f;

    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            uint32_t vertexIndex = i * m_Width + j;

            m_Vertices[vertexIndex].Position.x = (topLeftX + j) * m_VertexDistance; // to center int x-axis
            m_Vertices[vertexIndex].Position.z = (topLeftZ + i) * m_VertexDistance; // to center int z-axis
        }
    }
    CalculateNormals();
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(TerrainVertex));
}

void TerrainMesh::CalculateNoiseMap()
{
    if (m_NoiseMap.empty())
        return;
    // first valid value
    float minHeight = Evaluate(m_NoiseMap[0]) * m_Elevation;
    float maxHeight = minHeight;

    for (uint32_t i = 0; i < m_Height; i++)
    {
        for (uint32_t j = 0; j < m_Width; j++)
        {
            uint32_t vertexIndex = i * m_Width + j;
            float newYPos = Evaluate(m_NoiseMap[vertexIndex]) * m_Elevation;
            m_Vertices[vertexIndex].Position.y = newYPos;

            minHeight = minHeight > newYPos ? newYPos : minHeight;
            maxHeight = maxHeight < newYPos ? newYPos : maxHeight;
        }
    }
    m_MinHeight = minHeight;
    m_MaxHeight = maxHeight;

    // Calculate normals before sending data to the gpu
    CalculateNormals();
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(TerrainVertex));
}

void TerrainMesh::SetElevation(float elevation)
{
    if (m_Elevation == elevation)
        return;

    m_Elevation = elevation;
    if (m_NoiseMap.empty())
        return;

    CalculateNoiseMap();
}

void TerrainMesh::SetNoiseMap(const std::vector<float> &noiseMap)
{
    // Add a loggin system with asserts and macros
    // ASSERT(noiseMap.size() != m_Width * m_Height);
    // asserts are slow so keep it with the macros !
    m_NoiseMap = std::move(noiseMap);
    CalculateNoiseMap();
}

void TerrainMesh::SetWidth(uint32_t width)
{
    m_Width = width;

    m_Vertices.clear();
    m_Indices.clear();

    m_VertexBufferObject.reset();
    m_IndexBufferObject.reset();

    CreateTerrain();
}
void TerrainMesh::SetHeight(uint32_t height)
{
    m_Height = height;

    m_Vertices.clear();
    m_Indices.clear();
    m_VertexBufferObject.reset();
    m_IndexBufferObject.reset();

    CreateTerrain();
}

glm::vec3 TerrainMesh::GetNormalFromIndices(unsigned int indexA, unsigned int indexB, unsigned int indexC, bool flipDir)
{
    glm::vec3 pointA = m_Vertices[indexA].Position;
    glm::vec3 pointB = m_Vertices[indexB].Position;
    glm::vec3 pointC = m_Vertices[indexC].Position;

    glm::vec3 vectAB = pointB - pointA;
    glm::vec3 vectAC = pointC - pointA;
    if (flipDir)
        std::swap(vectAB, vectAC);
    return glm::normalize(glm::cross(vectAB, vectAC));
}

float TerrainMesh::Evaluate(float t)
{
    switch (m_ElevationCurve)
    {
    case AnimationCurve::Linear:
        return t;
    case AnimationCurve::EaseInQuad:
        return Enxus::Math::easeInQuad(t);
    case AnimationCurve::EaseInCubic:
        return Enxus::Math::easeInCubic(t);
    case AnimationCurve::EaseInQuart:
        return Enxus::Math::easeInQuart(t);
    case AnimationCurve::EaseInQuint:
        return Enxus::Math::easeInQuint(t);
    }
    std::cout << "[AnimationCurve Error] Unknown Animation Curve";
    ASSERT(false);
    // to avoid warning
    return AnimationCurve::Linear;
}
