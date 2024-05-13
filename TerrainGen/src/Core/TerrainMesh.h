#ifndef TERRAIN_MESH_H
#define TERRAIN_MESH_H

#include "Enxus.h"

struct TerrainVertex
{
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;

    TerrainVertex(glm::vec3 position)
        : Position(position), TexCoord(0), Normal(0) {}

    TerrainVertex(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal)
        : Position(position), TexCoord(texCoord), Normal(normal) {}
};

enum AnimationCurve
{
    Linear = 0,
    EaseInQuad,
    EaseInCubic,
    EaseInQuart,
    EaseInQuint,
};

class TerrainMesh
{
public:
    TerrainMesh() = delete;
    TerrainMesh(uint32_t width, uint32_t height);
    ~TerrainMesh();

    void SetNoiseMap(const std::vector<float> &noiseMap);
    void SetHeightElevationCurve(AnimationCurve curve);
    void SetElevation(float elevation);
    void SetVertexDistance(float distance);
    void SetWidth(uint32_t width);
    void SetHeight(uint32_t height);

    inline Enxus::Ref<Enxus::TextureMesh2D> GetGrassTexture() const { return m_GrassTexture; }
    inline Enxus::Ref<Enxus::TextureMesh2D> GetSnowTexture() const { return m_SnowTexture; }

    inline float GetMinHeight() const { return m_MinHeight; }
    inline float GetMaxHeight() const { return m_MaxHeight; }

    inline uint32_t GetWidth() const { return m_Width; }
    inline uint32_t GetHeight() const { return m_Height; }

    inline Enxus::Ref<Enxus::VertexArray> GetVertexArray() const { return m_VertexArrayObject; }
    inline Enxus::Ref<Enxus::IndexBuffer> GetIndexBuffer() const { return m_IndexBufferObject; }

private:
    void CreateTerrain();
    void CreateVertices();
    void CalculateNormals();
    void CalculateNoiseMap();
    // Indices for Triangle Strip
    std::vector<unsigned int> CreateIndices();

    glm::vec3 GetNormalFromIndices(unsigned int indexA, unsigned int indexB, unsigned int indexC, bool flipDir);
    float Evaluate(float t);

private:
    uint32_t m_Width, m_Height;
    // float m_VertexDistance = 0.0625f;
    float m_VertexDistance = 0.05f;
    float m_Elevation = 1.5f;
    float m_MinHeight = 0.0f;
    float m_MaxHeight = 1.0f;

    AnimationCurve m_ElevationCurve = AnimationCurve::EaseInQuad;

    std::vector<float> m_NoiseMap;

    std::vector<TerrainVertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

    Enxus::Ref<Enxus::TextureMesh2D> m_GrassTexture, m_SnowTexture;

    Enxus::Ref<Enxus::VertexArray> m_VertexArrayObject;
    Enxus::Ref<Enxus::VertexBuffer> m_VertexBufferObject;
    Enxus::Ref<Enxus::IndexBuffer> m_IndexBufferObject;

    static const uint32_t s_MaxTerrainSize;
};
#endif
