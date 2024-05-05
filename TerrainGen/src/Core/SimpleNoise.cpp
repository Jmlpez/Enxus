#include "pch.h"
#include "SimpleNoise.h"

inline float lerp(float lo, float hi, float t)
{
    return lo * (1 - t) + hi * t;
}

inline float smoothstep(float t)
{
    return t * t * (3 - 2 * t);
}

void SimpleNoise::Init(uint32_t seed)
{
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distrFloat;
    auto randFloat = std::bind(distrFloat, gen);
    // create an array of random values
    for (unsigned k = 0; k < c_MaxTableSize; ++k)
    {
        m_Rand[k] = randFloat();
        m_PermutationTable[k] = k;
    }

    // shuffle values of the permutation table
    std::uniform_int_distribution<unsigned> distrUInt;
    auto randUInt = std::bind(distrUInt, gen);
    for (unsigned k = 0; k < c_MaxTableSize; ++k)
    {
        unsigned i = randUInt() & c_MaxTableSizeMask;
        std::swap(m_PermutationTable[k], m_PermutationTable[i]);
        m_PermutationTable[k + c_MaxTableSize] = m_PermutationTable[k];
    }
}

float SimpleNoise::Eval(glm::vec2 p)
{
    const int xi = std::floor(p.x);
    int yi = std::floor(p.y);
    float tx = p.x - xi;
    float ty = p.y - yi;
    int rx0 = xi & c_MaxTableSizeMask;
    int rx1 = (rx0 + 1) & c_MaxTableSizeMask;
    int ry0 = yi & c_MaxTableSizeMask;
    int ry1 = (ry0 + 1) & c_MaxTableSizeMask;
    // random values at the corners of the cell using the permutation table

    float c00 = m_Rand[m_PermutationTable[m_PermutationTable[rx0] + ry0]];
    float c10 = m_Rand[m_PermutationTable[m_PermutationTable[rx1] + ry0]];
    float c01 = m_Rand[m_PermutationTable[m_PermutationTable[rx0] + ry1]];
    float c11 = m_Rand[m_PermutationTable[m_PermutationTable[rx1] + ry1]];

    // remapping of tx and ty using the Smoothstep function
    float sx = smoothstep(tx);
    float sy = smoothstep(ty);

    // linearly interpolate values along the x axis
    float nx0 = lerp(c00, c10, sx);
    float nx1 = lerp(c01, c11, sx);
    return lerp(nx0, nx1, sy);
}
