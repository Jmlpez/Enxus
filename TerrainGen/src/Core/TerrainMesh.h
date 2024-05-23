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
    void SetLevelOfDetail(int levelOfDetail);
    void SetWidth(uint32_t width);
    void SetHeight(uint32_t height);

    inline int GetLevelOfDetail() const { return m_LevelOfDetail; }

    inline float GetElevation() const { return m_Elevation; }
    inline float GetVertexDistance() const { return m_VertexDistance; }
    inline AnimationCurve GetHeightElevationCurve() const { return m_ElevationCurve; }

    inline float GetMinHeight() const { return m_MinHeight; }
    inline float GetMaxHeight() const { return m_MaxHeight; }

    inline uint32_t GetWidth() const { return m_Width; }
    inline uint32_t GetHeight() const { return m_Height; }

    inline Enxus::Ref<Enxus::VertexArray> GetVertexArray() const { return m_VertexArrayObject; }

    uint32_t GetSimplifiedWidth();
    uint32_t GetSimplifiedHeight();

private:
    void CreateTerrain();
    void CalculateNormals();
    void CalculateNoiseMap();

    std::vector<Enxus::VertexData> CreateVertices();
    // Indices for Triangle Strip
    std::vector<uint32_t> CreateIndices();

    float Evaluate(float t);
    uint32_t GetMeshSimplificationStep();
    uint32_t GetSimplifiedSize(uint32_t size, uint8_t step);

    glm::vec3 GetNormalFromIndices(uint32_t indexA, uint32_t indexB, uint32_t indexC, bool flipDir);

private:
    uint32_t m_Width, m_Height;
    float m_VertexDistance = 0.05f;
    float m_Elevation = 1.5f;
    float m_MinHeight = 0.0f;
    float m_MaxHeight = 1.0f;
    int m_LevelOfDetail;

    AnimationCurve m_ElevationCurve = AnimationCurve::EaseInQuad;

    std::vector<float> m_NoiseMap;

    std::vector<Enxus::VertexData> m_Vertices;
    std::vector<uint32_t> m_Indices;

    Enxus::Ref<Enxus::VertexArray> m_VertexArrayObject;
    Enxus::Ref<Enxus::VertexBuffer> m_VertexBufferObject;
    Enxus::Ref<Enxus::IndexBuffer> m_IndexBufferObject;

    static const uint32_t s_MaxTerrainSize;
};
#endif
