#include "pch.h"
#include "Erosion.h"

namespace Erosion
{
    //----------------- CONSTANTS -------------------//

    const int c_ErosionRadius = 3;
    const float c_ErosionRate = 0.3f;
    const float c_DepostionRate = 0.3f;
    const float c_EvaporationRate = 0.01f;
    const float c_SedimentCapacityRate = 4.0f;
    const float c_MinSlope = 0.01f;
    const float c_Inertia = 0.05f;
    const float c_Gravity = 4.0f;
    const uint32_t c_MaxDropLifeTime = 30;

    ErosionManager::ErosionManager(const std::vector<float> &heightMap, uint32_t width, uint32_t height)
        : m_HeightMap(heightMap),
          m_Width(width),
          m_Height(height),
          m_RandomGen(std::random_device()())
    {
        InitErosionWeight();
    }

    void ErosionManager::SetHeightMap(const std::vector<float> &heightMap)
    {
        m_HeightMap = heightMap;
        InitErosionWeight();
    }

    void ErosionManager::InitErosionWeight()
    {
        const uint32_t mapSize = m_Width * m_Height;
        if (m_ErosionAreaWeight.size() != mapSize)
        {
            m_ErosionAreaWeight.resize(mapSize);
            m_ErosionAreaIndices.resize(mapSize);
        }

        for (uint32_t i = 0; i < mapSize; i++)
        {
            int centerX = i % m_Width;
            int centerY = i / m_Height;
            // simulate a drop at position centerX, centerY

            float totalWeights = 0.0f;
            uint32_t currentIndex = 0;
            std::vector<float> weights(c_ErosionRadius * c_ErosionRadius * 4);
            std::vector<int> xOffset(c_ErosionRadius * c_ErosionRadius * 4), yOffset(c_ErosionRadius * c_ErosionRadius * 4);

            // if is in the boundaries of the terrain ignore it --> this prevent from erode the borders
            if ((uint32_t)centerY <= c_ErosionRadius ||
                (uint32_t)centerY >= m_Height - c_ErosionRadius ||
                (uint32_t)centerX <= c_ErosionRadius ||
                (uint32_t)centerX >= m_Width - c_ErosionRadius)
                continue;

            for (int y = -c_ErosionRadius; y <= c_ErosionRadius; y++)
            {
                for (int x = -c_ErosionRadius; x <= c_ErosionRadius; x++)
                {
                    int posX = centerX + x;
                    int posY = centerY + y;
                    float distance = (x * x + y * y);

                    if (!(posY >= 0 && posY < (int)m_Height && posX >= 0 && posX < (int)m_Width) || distance > c_ErosionRadius * c_ErosionRadius)
                        continue;

                    float weight = 1.0f - (glm::sqrt(distance) / c_ErosionRadius);
                    totalWeights += weight;

                    weights[currentIndex] = weight;
                    xOffset[currentIndex] = x;
                    yOffset[currentIndex] = y;
                    currentIndex++;
                }
            }

            m_ErosionAreaWeight[i].resize(currentIndex);
            m_ErosionAreaIndices[i].resize(currentIndex);
            for (uint32_t j = 0; j < currentIndex; j++)
            {
                m_ErosionAreaIndices[i][j] = Index(centerX + xOffset[j], centerY + yOffset[j]);
                m_ErosionAreaWeight[i][j] = weights[j] / totalWeights;
            }
        }
    }

    void ErosionManager::Simulate(uint32_t numOfIterations)
    {
        static std::uniform_int_distribution<int> randX(0, m_Width - 1);
        static std::uniform_int_distribution<int> randY(0, m_Height - 1);

        std::cout << "Begin simulation" << std::endl;
        for (uint32_t iterations = 0; iterations < numOfIterations; iterations++)
        {
            uint32_t randPosX = randX(m_RandomGen);
            uint32_t randPosY = randY(m_RandomGen);
            Drop drop(randPosX, randPosY);
            SimulateDrop(drop);
        }
    }

    void ErosionManager::SimulateDrop(Drop &drop)
    {
        for (uint32_t lifeTime = 1; lifeTime <= c_MaxDropLifeTime; lifeTime++)
        {

            int floorPosX = (int)drop.Position.x;
            int floorPosY = (int)drop.Position.y;

            float offsetX = drop.Position.x - floorPosX;
            float offsetY = drop.Position.y - floorPosY;

            uint32_t oldDropPosIndex = Index(floorPosX, floorPosY);

            // Get height and gradient according to the old drop position
            float oldHeight = GetInterpHeight(drop.Position);
            glm::vec2 gradient = GetInterpGradient(drop.Position);

            drop.Direction = drop.Direction * c_Inertia - gradient * (1 - c_Inertia);

            float length = glm::length(drop.Direction);
            if (length != 0.0f)
                drop.Direction /= length;

            drop.Position += drop.Direction;

            if (!IsInBoundaries(drop.Position) || (drop.Direction.x == 0 && drop.Direction.y == 0))
                break;

            // Get height according to the new drop position
            float newHeight = GetInterpHeight(drop.Position);

            float heightDiff = newHeight - oldHeight;

            // calculate sediment capacity
            float maximumSediment = std::max(-heightDiff, c_MinSlope) * drop.Speed * drop.Water * c_SedimentCapacityRate;

            if (drop.Sediment > maximumSediment || heightDiff > 0)
            {
                float amountToDeposit = (heightDiff > 0) ? std::min(heightDiff, drop.Sediment) : (drop.Sediment - maximumSediment) * c_DepostionRate;
                drop.Sediment -= amountToDeposit;

                //  use bilinear interpolation to deposit the sediment across the neighbor cells of the OLD POSITION
                m_HeightMap[oldDropPosIndex] += amountToDeposit * (1 - offsetX) * (1 - offsetY);
                m_HeightMap[oldDropPosIndex + 1] += amountToDeposit * offsetX * (1 - offsetY);
                m_HeightMap[oldDropPosIndex + m_Width] += amountToDeposit * (1 - offsetX) * offsetY;
                m_HeightMap[oldDropPosIndex + m_Width + 1] += amountToDeposit * offsetX * offsetY;
            }
            else
            {
                // erode the terrain
                float amountToErode = std::min((maximumSediment - drop.Sediment) * c_ErosionRate, -heightDiff);

                for (uint32_t i = 0; i < m_ErosionAreaIndices[oldDropPosIndex].size(); i++)
                {
                    uint32_t cellIdx = m_ErosionAreaIndices[oldDropPosIndex][i];
                    float weightedErodeAmount = amountToErode * m_ErosionAreaWeight[oldDropPosIndex][i];
                    // Because erosion remove height we dont want that height goes below 0
                    if (m_HeightMap[cellIdx] < weightedErodeAmount)
                        weightedErodeAmount = m_HeightMap[cellIdx];

                    m_HeightMap[cellIdx] -= weightedErodeAmount;
                    drop.Sediment += weightedErodeAmount;
                }
            }

            // Update speed and evaporate a tiny fraction of the water
            drop.Speed = glm::sqrt(std::max(0.001f, drop.Speed * drop.Speed + heightDiff * c_Gravity));
            drop.Water *= (1 - c_EvaporationRate);
        }
    }

    glm::vec2 ErosionManager::GetInterpGradient(glm::vec2 position)
    {
        int floorPosX = (int)position.x;
        int floorPosY = (int)position.y;

        float offsetX = position.x - floorPosX;
        float offsetY = position.y - floorPosY;

        uint32_t oldPosIndex = Index(floorPosX, floorPosY);

        uint32_t xy = oldPosIndex;
        uint32_t x1y = oldPosIndex + 1;
        uint32_t xy1 = oldPosIndex + m_Width;
        uint32_t x1y1 = oldPosIndex + m_Width + 1;

        float lerpGradX = (m_HeightMap[x1y] - m_HeightMap[xy]) * (1 - offsetY) + (m_HeightMap[x1y1] - m_HeightMap[xy1]) * offsetY;
        float lerpGradY = (m_HeightMap[xy1] - m_HeightMap[xy]) * (1 - offsetX) + (m_HeightMap[x1y1] - m_HeightMap[x1y]) * offsetX;

        return glm::vec2(lerpGradX, lerpGradY);
    }

    float ErosionManager::GetInterpHeight(glm::vec2 position)
    {
        int floorPosX = (int)position.x;
        int floorPosY = (int)position.y;

        float offsetX = position.x - floorPosX;
        float offsetY = position.y - floorPosY;

        uint32_t oldPosIndex = Index(floorPosX, floorPosY);

        uint32_t xy = oldPosIndex;
        uint32_t x1y = oldPosIndex + 1;
        uint32_t xy1 = oldPosIndex + m_Width;
        uint32_t x1y1 = oldPosIndex + m_Width + 1;

        float heightXY = m_HeightMap[xy] * (1 - offsetX) * (1 - offsetY);
        float heightXY1 = m_HeightMap[xy1] * (1 - offsetX) * offsetY;
        float heightX1Y = m_HeightMap[x1y] * offsetX * (1 - offsetY);
        float heightX1Y1 = m_HeightMap[x1y1] * offsetX * offsetY;

        return heightXY + heightXY1 + heightX1Y + heightX1Y1;
    }

    bool ErosionManager::IsInBoundaries(glm::vec2 position)
    {
        return position.x >= 0.0f && position.x < m_Width && position.y >= 0.0f && position.y < m_Height;
    }

}
