#ifndef EROSION_H
#define EROSION_H

#include "Enxus.h"

namespace Erosion
{
    struct Drop
    {
        glm::vec2 Position;
        glm::vec2 Direction = glm::vec2(0, 0);

        float Water = 1.0f; // The volume
        float Speed = 1.0f;
        float Sediment = 0.0f;

        Drop() : Position(glm::vec2(0.0f, 0.0f)) {}
        Drop(glm::vec2 position) : Position(position) {}
        Drop(float x, float y) : Position(glm::vec2(x, y)) {}
    };

    class ErosionManager
    {
    public:
        ErosionManager(const std::vector<float> &heightMap, uint32_t width, uint32_t height);

        void SetHeightMap(const std::vector<float> &heightMap);

        void Simulate(uint32_t numOfIterations = 1000);
        inline const std::vector<float> &GetHeightMap() const { return m_HeightMap; }

    private:
        inline uint32_t Index(uint32_t x, uint32_t y) const { return y * m_Width + x; }
        glm::vec2 GetInterpGradient(glm::vec2 position);
        float GetInterpHeight(glm::vec2 position);
        bool IsInBoundaries(glm::vec2 position);
        void InitErosionWeight();
        void SimulateDrop(Drop &drop);

    private:
        std::vector<float> m_HeightMap;
        std::vector<std::vector<float>> m_ErosionAreaWeight;
        std::vector<std::vector<uint32_t>> m_ErosionAreaIndices;

        uint32_t m_Width, m_Height;

        std::mt19937 m_RandomGen;
    };

}

#endif