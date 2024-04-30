#include "pch.h"
#include "HeightMapTerrain.h"
#include "stb_image/stb_image.h"

HeightMapTerrain::HeightMapTerrain()
{
    m_VertexArrayObject = Enxus::CreateRef<Enxus::VertexArray>();
}

HeightMapTerrain::HeightMapTerrain(const std::string &heightMap)
    : m_HeightMapPath(heightMap)
{
    m_VertexArrayObject = Enxus::CreateRef<Enxus::VertexArray>();
    LoadHeightMap(heightMap);
}

HeightMapTerrain::~HeightMapTerrain()
{
}

void HeightMapTerrain::SetHeightMap(const std::string &heightMap)
{
    m_HeightMapPath = heightMap;
    LoadHeightMap(m_HeightMapPath);
}

void HeightMapTerrain::LoadHeightMap(const std::string &heightMap)
{
    // stbi_set_flip_vertically_on_load(true);
    m_HeightMapData = stbi_load(heightMap.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
    if (!m_HeightMapData)
    {
        std::cout << "[HeightMapTerrain Error]: Failed to load height map texture: " << heightMap << std::endl;
        stbi_image_free(m_HeightMapData);
        return;
    }
    CreateTerrain(m_Width, m_Height);

    stbi_image_free(m_HeightMapData);
}

void HeightMapTerrain::CreateTerrain(int width, int height)
{
    std::vector<glm::vec3> vertices = CreateVertices(width, height);
    ASSERT(vertices.size() > 0);
    m_VertexBufferObject = Enxus::CreateRef<Enxus::VertexBuffer>(&vertices[0], vertices.size() * sizeof(glm::vec3));

    /*
        Grid Vertex Layout
        x, y, z --> position
    */
    Enxus::VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);

    m_VertexArrayObject->AddBuffer(*m_VertexBufferObject, layout);

    std::vector<unsigned int>
        indices = CreateIndices(width, height);

    m_IndexBufferObject = Enxus::CreateRef<Enxus::IndexBuffer>(&indices[0], indices.size());
}

std::vector<glm::vec3> HeightMapTerrain::CreateVertices(int width, int height)
{
    std::vector<glm::vec3> vertices;
    // vertices.reserve(width * height * 3);

    // the scale factor is to be in range [0, 4.0f] and the shift to allow negative values i.e [-0.84f, 3.84f]
    float heightScaleFactor = 1.0f, heightShift = 0.016f;

    // the distance between vertices will be 1 unit

    float vertexDistance = 0.0625f;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            glm::vec3 position;
            position.x = (-(float)width / 2.0f + i) * vertexDistance;
            // the bytes are consecutive in memory
            unsigned char *texel = m_HeightMapData + (i * width + j) * m_NrChannels;
            position.y = (((int)texel[0] / 256.0f) * heightScaleFactor) - heightShift; // it could be texel[1|2] anyway because its a heightmap
            position.z = (-(float)height / 2.0f + j) * vertexDistance;
            vertices.push_back(position);
        }
    }

    return vertices;
}

std::vector<unsigned int> HeightMapTerrain::CreateIndices(int width, int height)
{
    std::vector<unsigned int> indices((height - 1) * (width * 2), 0);
    int index = 0;
    for (int i = 0; i < height - 1; i++)
    {
        int offset = width * i;
        indices[index] = offset;
        indices[index + 1] = offset + width;
        index += 2;
        for (int j = 1; j < width; j++)
        {
            indices[index] = offset + j;
            indices[index + 1] = offset + j + width;
            // indices[index + 2] = offset + j + 1;
            // indices[index + 3] = offset + j + width + 1;
            index += 2;
        }
    }

    return indices;
}
