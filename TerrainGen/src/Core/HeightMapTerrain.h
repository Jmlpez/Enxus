#ifndef HEIGHT_MAP_TERRAIN_H
#define HEIGHT_MAP_TERRAIN_H

#include "Enxus.h"

class HeightMapTerrain
{
public:
    HeightMapTerrain();
    HeightMapTerrain(const std::string &heightMap);
    ~HeightMapTerrain();

    void SetHeightMap(const std::string &heightMap);
    inline Enxus::Ref<Enxus::VertexArray> GetVertexArray() const { return m_VertexArrayObject; }
    inline Enxus::Ref<Enxus::VertexBuffer> GetVertexBuffer() const { return m_VertexBufferObject; }
    inline Enxus::Ref<Enxus::IndexBuffer> GetIndexBuffer() const { return m_IndexBufferObject; }

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

private:
    void LoadHeightMap(const std::string &heightMap);
    void CreateTerrain(int width, int height);
    std::vector<glm::vec3> CreateVertices(int width, int height);
    std::vector<unsigned int> CreateIndices(int width, int height);

    // void CalculateModel();

private:
    int m_Width, m_Height, m_NrChannels;

    unsigned char *m_HeightMapData;
    std::string m_HeightMapPath;

    Enxus::Ref<Enxus::VertexArray> m_VertexArrayObject;
    Enxus::Ref<Enxus::VertexBuffer> m_VertexBufferObject;
    Enxus::Ref<Enxus::IndexBuffer> m_IndexBufferObject;
};
#endif