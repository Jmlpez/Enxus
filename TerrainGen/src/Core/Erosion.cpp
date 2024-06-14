#include "pch.h"
#include "Erosion.h"

namespace Erosion
{
    //----------------- CONSTANTS -------------------//

    ErosionManager::ErosionManager()
        : m_Width(0), m_Height(0), m_RandomGen(std::random_device()())
    {
    }

    ErosionManager::ErosionManager(const std::vector<float> &heightMap, uint32_t width, uint32_t height)
        : m_HeightMap(heightMap),
          m_Width(width),
          m_Height(height),
          m_RandomGen(std::random_device()())
    {
        InitErosionWeight();
    }

    void ErosionManager::SetMapSize(uint32_t width, uint32_t height)
    {
        m_Width = width;
        m_Height = height;
        m_ErosionAreaIndices.clear();
        m_ErosionAreaWeight.clear();
        InitErosionWeight();
    }

    void ErosionManager::SetHeightMap(const std::vector<float> &heightMap)
    {
        // restart animation each time the height map is set
        m_HeightMap = heightMap;
        m_CurrentIterations = 0;
    }

    void ErosionManager::Simulate(uint32_t numOfIterations)
    {
        if (m_HeightMap.size() == 0)
            return;
        if (m_Width == 0 || m_Height == 0)
        {
            std::cout << "[ErosionManager Error]: The width or the weight cannot be 0" << std::endl;
            ASSERT(false);
        }
        static std::uniform_int_distribution<int> randX(1, m_Width - 2);
        static std::uniform_int_distribution<int> randY(1, m_Height - 2);
        for (uint32_t iterations = 0; iterations < numOfIterations; iterations++)
        {
            uint32_t randPosX = randX(m_RandomGen);
            uint32_t randPosY = randY(m_RandomGen);
            Drop drop(randPosX, randPosY);
            SimulateDrop(drop);
        }

        m_CurrentIterations += numOfIterations;
    }

    void ErosionManager::SimulateDrop(Drop &drop)
    {
        for (uint32_t lifeTime = 1; lifeTime <= m_Constants.MaxDropLifeTime; lifeTime++)
        {

            int floorPosX = (int)drop.Position.x;
            int floorPosY = (int)drop.Position.y;

            float offsetX = drop.Position.x - floorPosX;
            float offsetY = drop.Position.y - floorPosY;

            uint32_t oldDropPosIndex = Index(floorPosX, floorPosY);

            // Get height and gradient according to the old drop position
            float oldHeight = GetInterpHeight(drop.Position);
            glm::vec2 gradient = GetInterpGradient(drop.Position);

            drop.Direction = drop.Direction * m_Constants.Inertia - gradient * (1 - m_Constants.Inertia);

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
            float maximumSediment = std::max(-heightDiff, m_Constants.MinSlope) * drop.Speed * drop.Water * m_Constants.SedimentCapacityRate;

            if (drop.Sediment > maximumSediment || heightDiff > 0)
            {
                float amountToDeposit = (heightDiff > 0) ? std::min(heightDiff, drop.Sediment) : (drop.Sediment - maximumSediment) * m_Constants.DepostionRate;
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
                float amountToErode = std::min((maximumSediment - drop.Sediment) * m_Constants.ErosionRate, -heightDiff);

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
            drop.Speed = glm::sqrt(std::max(0.001f, drop.Speed * drop.Speed + heightDiff * m_Constants.Gravity));
            drop.Water *= (1 - m_Constants.EvaporationRate);
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

        if (floorPosX >= (int)m_Width - 1 || floorPosY >= (int)m_Height - 1)
        {
            return glm::vec2(0, 0);
        }

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

        if (floorPosX >= (int)m_Width - 1 || floorPosY >= (int)m_Height - 1)
        {
            return 0.0f;
        }

        float heightXY = m_HeightMap[xy] * (1 - offsetX) * (1 - offsetY);
        float heightXY1 = m_HeightMap[xy1] * (1 - offsetX) * offsetY;
        float heightX1Y = m_HeightMap[x1y] * offsetX * (1 - offsetY);
        float heightX1Y1 = m_HeightMap[x1y1] * offsetX * offsetY;

        return heightXY + heightXY1 + heightX1Y + heightX1Y1;
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
            std::vector<float> weights(m_Constants.ErosionRadius * m_Constants.ErosionRadius * 4);
            std::vector<int> xOffset(m_Constants.ErosionRadius * m_Constants.ErosionRadius * 4), yOffset(m_Constants.ErosionRadius * m_Constants.ErosionRadius * 4);

            // if is in the boundaries of the terrain ignore it --> this prevent from erode the borders
            if (centerY <= m_Constants.ErosionRadius ||
                centerY >= (int)m_Height - m_Constants.ErosionRadius ||
                centerX <= m_Constants.ErosionRadius ||
                centerX >= (int)m_Width - m_Constants.ErosionRadius)
                continue;
            for (int y = -m_Constants.ErosionRadius; y <= m_Constants.ErosionRadius; y++)
            {
                for (int x = -m_Constants.ErosionRadius; x <= m_Constants.ErosionRadius; x++)
                {
                    int posX = centerX + x;
                    int posY = centerY + y;
                    float distance = (x * x + y * y);

                    if (!(posY >= 0 && posY < (int)m_Height - 1 && posX >= 0 && posX < (int)m_Width - 1) || distance > m_Constants.ErosionRadius * m_Constants.ErosionRadius)
                        continue;

                    float weight = 1.0f - (glm::sqrt(distance) / m_Constants.ErosionRadius);
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

    bool ErosionManager::IsInBoundaries(glm::vec2 position)
    {
        return position.x >= 0.0f && position.x < m_Width - 1 && position.y >= 0.0f && position.y < m_Height - 1;
    }

}
