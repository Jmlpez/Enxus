
#ifndef POISSON_DISK_SAMPLER_H
#define POISSON_DISK_SAMPLER_H

#include "Enxus.h"

class PoissonDiskSampler
{
public:
    PoissonDiskSampler(uint32_t width, uint32_t height, float radius, uint32_t amount);
    ~PoissonDiskSampler(){};

    inline float GetRadius() const { return m_Radius; }
    inline uint32_t GetAmount() const { return m_Amount; }
    inline const std::vector<glm::vec2> &GetSampledPoints() const { return m_FinalPointsList; }

    void SetSeed(uint32_t seed);
    void SetRadius(float radius);
    void SetAmount(uint32_t amount);
    void CalculatePoints();

private:
    void InitGrid();
    void ResetValues();
    void AddPoint(glm::vec2 point);
    bool IsInBounds(glm::vec2 point);
    bool CanPlacePoint(glm::vec2 point);
    glm::vec2 GeneratePointAround(glm::vec2 newPoint);

private:
    uint32_t m_Width, m_Height;
    float m_Radius, m_CellSize;
    uint32_t m_Amount;

    std::vector<glm::vec2> m_PointsQueue, m_FinalPointsList;

    uint32_t m_GridRows, m_GridCols;
    std::vector<std::vector<glm::vec2>> m_Grid;

    // uint32_fast m_Seed;
    uint32_t m_Seed;
    // uint_fas
    std::mt19937 m_RandomGenEngine;
};

#endif