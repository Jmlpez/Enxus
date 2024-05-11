#ifndef TERRAIN_MESH_H
#define TERRAIN_MESH_H

#include "Enxus.h"

class TerrainMesh
{
public:
    TerrainMesh();
    TerrainMesh(uint32_t width, uint32_t height);
    ~TerrainMesh();

    void SetNoiseMap(const std::vector<float> &noiseMap);
    void SetHeightScaleFactor(float heightScale);
    void SetVertexDistance(float distance);
    void SetWidth(uint32_t width);
    void SetHeight(uint32_t height);

    inline uint32_t GetWidth() const { return m_Width; }
    inline uint32_t GetHeight() const { return m_Height; }

    inline Enxus::Ref<Enxus::VertexArray> GetVertexArray() const { return m_VertexArrayObject; }
    inline Enxus::Ref<Enxus::IndexBuffer> GetIndexBuffer() const { return m_IndexBufferObject; }

private:
    void CreateTerrain();
    void CreateVertices();
    // Indices for Triangle Strip
    std::vector<unsigned int> CreateIndices();

private:
    uint32_t m_Width, m_Height;
    // float m_VertexDistance = 0.0625f;
    float m_VertexDistance = 0.05f;
    float m_HeightScale = 1.5f;

    std::vector<float> m_NoiseMap;

    std::vector<glm::vec3> m_Vertices;
    std::vector<unsigned int> m_Indices;

    // Enxus::Ref<Enxus::Mesh> m_Terrain;
    Enxus::Ref<Enxus::VertexArray> m_VertexArrayObject;
    Enxus::Ref<Enxus::VertexBuffer> m_VertexBufferObject;
    Enxus::Ref<Enxus::IndexBuffer> m_IndexBufferObject;

    static const uint32_t s_MaxTerrainSize;
};
#endif