#include "TerrainMesh.h"

const uint32_t TerrainMesh::s_MaxTerrainSize = 500;

TerrainMesh::TerrainMesh(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height), m_HeightCurve(Enxus::AnimationCurveType::Linear)
{
    if (m_Width >= s_MaxTerrainSize || m_Height >= s_MaxTerrainSize)
    {
        std::cout << "[TerrainMesh Warning] Max size allowed is  " << s_MaxTerrainSize << std::endl;
    }
    m_Width = std::min(m_Height, s_MaxTerrainSize);
    m_Height = std::min(m_Width, s_MaxTerrainSize);

    m_LevelOfDetail = 0;

    CreateTerrain();
}

TerrainMesh::~TerrainMesh()
{
}

void TerrainMesh::Draw() const
{
    // Draw the terrain using triangle strips
    m_VertexArrayObject->Bind();

    uint32_t simplifiedWidth = GetSimplifiedWidth();
    uint32_t simplifiedHeight = GetSimplifiedHeight();

    const uint32_t numOfStrips = simplifiedHeight - 1;
    const uint32_t numOfVertPerStrip = simplifiedWidth * 2;
    for (unsigned int strip = 0; strip < numOfStrips; strip++)
    {
        size_t stripOffset = strip * numOfVertPerStrip * sizeof(unsigned int);
        GLCall(glDrawElements(GL_TRIANGLE_STRIP, numOfVertPerStrip, GL_UNSIGNED_INT, (void *)stripOffset));
    }
}

void TerrainMesh::CreateTerrain()
{
    m_VertexArrayObject = Enxus::CreateRef<Enxus::VertexArray>();

    m_Vertices = std::move(CreateVertices());
    m_VertexBufferObject = Enxus::CreateRef<Enxus::VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Enxus::VertexData));

    /*
        Terrain Vertex Layout
        x, y, z --> position
        x, y --> texture coordinates
        x, y, z --> normal vector
    */
    Enxus::BufferLayout layout = {
        {Enxus::ShaderDataType::Float3, "aPos"},
        {Enxus::ShaderDataType::Float2, "aTexCoord"},
        {Enxus::ShaderDataType::Float3, "aNormal"},
    };

    m_VertexBufferObject->SetLayout(layout);

    m_VertexArrayObject->ResetVertexAttribIndex();
    m_VertexArrayObject->AddVertexBuffer(m_VertexBufferObject);

    m_Indices = std::move(CreateIndices());
    m_IndexBufferObject = Enxus::CreateRef<Enxus::IndexBuffer>(&m_Indices[0], m_Indices.size());
}

std::vector<Enxus::VertexData> TerrainMesh::CreateVertices()
{

    float topLeftX = (float)m_Width / -2.0f;
    float topLeftZ = (float)m_Height / -2.0f;

    uint32_t simplificationStep = GetMeshSimplificationStep();
    uint32_t simplifiedWidth = GetSimplifiedSize(m_Width, simplificationStep);
    uint32_t simplifiedHeight = GetSimplifiedSize(m_Height, simplificationStep);

    std::vector<Enxus::VertexData> vertices;
    vertices.reserve(simplifiedHeight * simplifiedWidth);

    for (uint32_t i = 0; i < m_Height; i += simplificationStep)
    {
        for (uint32_t j = 0; j < m_Width; j += simplificationStep)
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
    return vertices;
}

std::vector<uint32_t> TerrainMesh::CreateIndices()
{
    uint32_t simplifiedWidth = GetSimplifiedWidth();
    uint32_t simplifiedHeight = GetSimplifiedHeight();

    std::vector<uint32_t> indices((simplifiedHeight - 1) * (simplifiedWidth * 2), 0);

    int index = 0;

    // Triangle strip indices
    for (uint32_t i = 0; i < simplifiedHeight - 1; i++)
    {
        int offset = simplifiedWidth * i;
        for (uint32_t j = 0; j < simplifiedWidth; j++)
        {
            indices[index] = offset + j;
            indices[index + 1] = offset + j + simplifiedWidth;
            index += 2;
        }
    }
    return indices;
}

void TerrainMesh::CalculateNormals()
{
    // Reset  Enxus::VertexData normals to 0
    for (auto &vertex : m_Vertices)
        vertex.Normal = glm::vec3(0);

    uint32_t simplifiedWidth = GetSimplifiedWidth();
    uint32_t simplifiedHeight = GetSimplifiedHeight();

    uint32_t numOfStrips = simplifiedHeight - 1;
    for (uint32_t strip = 0; strip < numOfStrips; strip++)
    {
        uint32_t numOfVertexPerStrip = simplifiedWidth * 2;
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

    // Normalize the Enxus::VertexData normals
    for (auto &vertex : m_Vertices)
        vertex.Normal = glm::normalize(vertex.Normal);
}

void TerrainMesh::SetHeightElevationCurve(AnimationCurve curve)
{
    m_ElevationCurve = curve;
    CalculateNoiseMap();
}

void TerrainMesh::SetHeightCurve(Enxus::AnimationCurveEditor curve)
{
    m_HeightCurve = curve;
    CalculateNoiseMap();
}

void TerrainMesh::SetVertexDistance(float distance)
{
    if (distance == m_VertexDistance)
        return;
    m_VertexDistance = distance;

    float topLeftX = (float)m_Width / -2.0f;
    float topLeftZ = (float)m_Height / -2.0f;

    uint32_t simplificationStep = GetMeshSimplificationStep();

    for (uint32_t i = 0, vertexIndex = 0; i < m_Height; i += simplificationStep)
    {
        for (uint32_t j = 0; j < m_Width; j += simplificationStep)
        {
            m_Vertices[vertexIndex].Position.x = (topLeftX + j) * m_VertexDistance; // to center int x-axis
            m_Vertices[vertexIndex].Position.z = (topLeftZ + i) * m_VertexDistance; // to center int z-axis
            vertexIndex++;
        }
    }
    CalculateNormals();
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(Enxus::VertexData));
}

void TerrainMesh::CalculateNoiseMap()
{

    if (m_NoiseMap.empty())
        return;
    // first valid value
    float minHeight = Evaluate(m_NoiseMap[0]) * m_Elevation;
    float maxHeight = minHeight;

    uint32_t simplificationStep = GetMeshSimplificationStep();
    uint32_t simplifiedWidth = GetSimplifiedSize(m_Width, simplificationStep);
    uint32_t simplifiedHeight = GetSimplifiedSize(m_Height, simplificationStep);

    for (uint32_t i = 0; i < simplifiedHeight; i++)
    {
        for (uint32_t j = 0; j < simplifiedWidth; j++)
        {
            uint32_t vertexIndex = (i * simplifiedWidth + j);
            uint32_t noiseIndex = (i * m_Width + j) * simplificationStep;
            float newYPos = Evaluate(m_NoiseMap[noiseIndex]) * m_Elevation;
            m_Vertices[vertexIndex].Position.y = newYPos;

            minHeight = minHeight > newYPos ? newYPos : minHeight;
            maxHeight = maxHeight < newYPos ? newYPos : maxHeight;
        }
    }
    m_MinHeight = minHeight;
    m_MaxHeight = maxHeight;

    // Calculate normals before sending data to the gpu
    CalculateNormals();
    m_VertexBufferObject->SetData(&m_Vertices[0], m_Vertices.size() * sizeof(Enxus::VertexData));
}

void TerrainMesh::SetElevation(float elevation)
{
    if (m_Elevation == elevation || m_NoiseMap.empty())
        return;
    if (elevation == 0)
        elevation = 0.001f; // small value to visualize the noise
    m_Elevation = elevation;

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

void TerrainMesh::SetLevelOfDetail(int levelOfDetail)
{
    m_LevelOfDetail = levelOfDetail;

    m_Vertices.clear();
    m_Indices.clear();
    m_VertexBufferObject.reset();
    m_IndexBufferObject.reset();
    m_VertexArrayObject.reset();

    CreateTerrain();
    CalculateNoiseMap();
}

glm::vec3 TerrainMesh::GetVertexFromCoords(uint32_t x, uint32_t y) const
{
    if (x > m_Width || y > m_Height)
    {
        std::cout << "[TerrainMesh Error]: x: " << x << ", y: " << y << " has invalid values" << std::endl;
        ASSERT(false);
    }

    uint32_t step = GetMeshSimplificationStep();
    x /= step;
    y /= step;

    uint32_t height = GetSimplifiedHeight();

    uint32_t index = y * height + x;

    return m_Vertices[index].Position;
}

glm::vec3 TerrainMesh::GetNormalFromIndices(uint32_t indexA, uint32_t indexB, uint32_t indexC, bool flipDir)
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
    return m_HeightCurve.Evaluate(t);

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

uint32_t TerrainMesh::GetSimplifiedWidth() const
{
    uint32_t step = GetMeshSimplificationStep();
    return GetSimplifiedSize(m_Width, step);
}

uint32_t TerrainMesh::GetSimplifiedHeight() const
{
    uint32_t step = GetMeshSimplificationStep();
    return GetSimplifiedSize(m_Height, step);
}

uint32_t TerrainMesh::GetMeshSimplificationStep() const
{
    // there is no simplification
    if (m_LevelOfDetail == 0)
        return 1;
    return m_LevelOfDetail << 1; // 2x
}
uint32_t TerrainMesh::GetSimplifiedSize(uint32_t size, uint8_t step) const
{
    if (step <= 0)
    {
        std::cout << "[TerrainMesh Error]: step cannot be less or equal than 0" << std::endl;
        ASSERT(false);
        return 0;
    }
    return ((size - 1) / step) + 1;
}
