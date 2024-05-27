#include "PoissonDiskSampler.h"

static const glm::vec2 EMPTY_CELL = glm::vec2(-1, -1);

PoissonDiskSampler::PoissonDiskSampler(uint32_t width, uint32_t height, float radius, uint32_t amount)
    : m_Width(width), m_Height(height), m_Radius(radius), m_Amount(amount),
      m_RandomGenEngine(m_RD())
{
    InitGrid();
}

void PoissonDiskSampler::InitGrid()
{
    m_CellSize = m_Radius / glm::root_two<float>();

    m_GridRows = (int)(m_Height / m_CellSize) + 1;
    m_GridCols = (int)(m_Width / m_CellSize) + 1;

    m_Grid = std::vector<std::vector<glm::vec2>>(m_GridRows, std::vector<glm::vec2>(m_GridCols, EMPTY_CELL));

    std::uniform_int_distribution<uint32_t> randIntWidth(0, m_Width);
    std::uniform_int_distribution<uint32_t> randIntHeight(0, m_Height);

    glm::ivec2 initial(randIntWidth(m_RandomGenEngine), randIntHeight(m_RandomGenEngine));
    AddPoint(initial);
}

void PoissonDiskSampler::CalculatePoints()
{
    static const uint32_t s_MaxAttemps = 30;
    uint32_t currentAdded = 1;
    while (!m_PointsQueue.empty() && currentAdded < m_Amount)
    {
        int index = std::uniform_int_distribution<int>(0, m_PointsQueue.size() - 1)(m_RandomGenEngine);
        bool found = false;
        for (uint32_t k = 0; k < s_MaxAttemps; k++)
        {
            glm::vec2 randPoint = GeneratePointAround(m_PointsQueue[index]);
            if (IsInBounds(randPoint) && CanPlacePoint(randPoint))
            {
                AddPoint(randPoint);
                currentAdded++;
                found = true;
                break;
            }
        }
        if (!found)
            m_PointsQueue.erase(m_PointsQueue.begin() + index);
    }
}

void PoissonDiskSampler::AddPoint(glm::vec2 point)
{
    m_PointsQueue.push_back(point);
    m_FinalPointsList.push_back(point);

    int col = (int)(point.x / m_CellSize);
    int row = (int)(point.y / m_CellSize);
    m_Grid[row][col] = point;
}
bool PoissonDiskSampler::IsInBounds(glm::vec2 point)
{
    return point.x >= 0 && point.x <= m_Width && point.y >= 0 && point.y <= m_Height;
}
bool PoissonDiskSampler::CanPlacePoint(glm::vec2 newPoint)
{
    int col = int(newPoint.x / m_CellSize);
    int row = int(newPoint.y / m_CellSize);
    for (int i = std::max(row - 2, 0); i < std::min(row + 3, (int)m_GridRows); ++i)
    {
        for (int j = std::max(col - 2, 0); j < std::min(col + 3, (int)m_GridCols); ++j)
        {
            glm::vec2 point = m_Grid[i][j];
            if (point != EMPTY_CELL && glm::distance(point, newPoint) < m_Radius)
                return false;
        }
    }
    return true;
}
glm::vec2 PoissonDiskSampler::GeneratePointAround(glm::vec2 point)
{
    float random01 = std::uniform_real_distribution<float>(0.0f, 1.0f)(m_RandomGenEngine);
    float random12 = std::uniform_real_distribution<float>(1.0f, 2.0f)(m_RandomGenEngine);

    float r = m_Radius * random12;
    float angle = glm::two_pi<float>() * random01;
    return point + glm::vec2(r * std::cos(angle), r * std::sin(angle));
}

void PoissonDiskSampler::SetRadius(float radius)
{
    m_Radius = radius;
    ResetValues();
    InitGrid();
    CalculatePoints();
}
void PoissonDiskSampler::SetAmount(uint32_t amount)
{
    m_Amount = amount;
    ResetValues();
    InitGrid();
    CalculatePoints();
}

void PoissonDiskSampler::ResetValues()
{
    m_FinalPointsList.clear();
    m_PointsQueue.clear();
    for (auto &rows : m_Grid)
        rows.clear();
    m_Grid.clear();
}
