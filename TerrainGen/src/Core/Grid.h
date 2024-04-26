#ifndef GRID_H
#define GRID_H

#include "Enxus.h"

class Grid
{
public:
    Grid(uint32_t rows, uint32_t cols, float scalingFactor = 1.0f);
    ~Grid();

    glm::mat4 GetModel() const { return m_GridModel; }

    Enxus::Ref<Enxus::VertexArray> GetVertexArray() const { return m_VertexArrayObject; }
    Enxus::Ref<Enxus::VertexBuffer> GetVertexBuffer() const { return m_VertexBufferObject; }
    Enxus::Ref<Enxus::IndexBuffer> GetIndexBuffer() const { return m_IndexBufferObject; }

private:
    void CreateGrid(uint32_t rows, uint32_t cols);
    std::vector<float> CreateVertices(uint32_t rows, uint32_t cols);
    std::vector<unsigned int> CreateIndices(uint32_t rows, uint32_t cols);

    void CalculateModel();

private:
    uint32_t m_Rows, m_Cols;
    float m_ScalingFactor = 1.0f;
    glm::mat4 m_GridModel;

    Enxus::Ref<Enxus::VertexArray> m_VertexArrayObject;
    Enxus::Ref<Enxus::VertexBuffer> m_VertexBufferObject;
    Enxus::Ref<Enxus::IndexBuffer> m_IndexBufferObject;
};
#endif