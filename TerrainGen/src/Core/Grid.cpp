#include "pch.h"
#include "Grid.h"

Grid::Grid(uint32_t rows, uint32_t cols, float scalingFactor)
    : m_Rows(rows), m_Cols(cols), m_ScalingFactor(scalingFactor), m_GridModel(1.0f)
{
    m_VertexArrayObject = Enxus::CreateRef<Enxus::VertexArray>();
    CreateGrid(rows, cols);
}
Grid::~Grid()
{
}

void Grid::CreateGrid(uint32_t rows, uint32_t cols)
{
    m_Rows = rows;
    m_Cols = cols;

    std::vector<float> vertices = CreateVertices(rows, cols);
    m_VertexBufferObject = Enxus::CreateRef<Enxus::VertexBuffer>(&vertices[0], vertices.size() * sizeof(float));

    /*
        Grid Vertex Layout
        x, y, z --> position
    */
    Enxus::VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);
    m_VertexArrayObject->AddBuffer(*m_VertexBufferObject, layout);

    std::vector<unsigned int>
        indices = CreateIndices(rows, cols);

    m_IndexBufferObject = Enxus::CreateRef<Enxus::IndexBuffer>(&indices[0], indices.size());

    CalculateModel();
}

std::vector<float> Grid::CreateVertices(uint32_t rows, uint32_t cols)
{

    std::vector<float> vertices;
    for (uint32_t i = 0; i <= rows; i++)
    {
        for (uint32_t j = 0; j <= cols; j++)
        {
            glm::vec3 pos;
            pos.x = j * m_ScalingFactor;
            pos.y = 0.0f;
            pos.z = i * m_ScalingFactor;

            vertices.push_back(pos.x);
            vertices.push_back(pos.y);
            vertices.push_back(pos.z);
        }
    }
    return vertices;
}

std::vector<unsigned int> Grid::CreateIndices(uint32_t rows, uint32_t cols)
{
    /*
    Indices Layout:
    0    1    2    3
    *----*----*----*
    |    |    |    |
   4|   5|   6|   7|
    *----*----*----*
    |    |    |    |
    |    |    |    |
    *----*----*----*
    8    9   10   11
    */

    std::vector<unsigned int> indices(rows * cols * 3 * 2, 0);
    uint32_t index = 0;
    for (uint32_t i = 0; i < rows; i++)
    {
        for (uint32_t j = 0; j < cols; j++)
        {
            uint32_t offset = (cols + 1) * i;
            // first triangle
            indices[index] = offset + j;
            indices[index + 1] = offset + j + 1;
            indices[index + 2] = offset + j + cols + 1;

            // second triangle
            indices[index + 3] = offset + j + 1;
            indices[index + 4] = offset + j + cols + 1;
            indices[index + 5] = offset + j + cols + 2;
            index += 6;
        }
    }
    return indices;
}

void Grid::CalculateModel()
{
    glm::vec3 translation(1.0f);
    translation.x = (float)m_Cols / 2.0f;
    translation.y = 0.0f;
    translation.z = (float)m_Rows / 2.0f;

    translation.x *= -m_ScalingFactor;
    translation.z *= -m_ScalingFactor;
    m_GridModel = glm::translate(m_GridModel, translation);
}
