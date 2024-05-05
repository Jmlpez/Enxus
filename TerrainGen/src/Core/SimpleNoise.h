#ifndef SIMPLE_NOISE_H
#define SIMPLE_NOISE_H

#include "Enxus.h"

// Math utilities
float lerp(float lo, float hi, float t);
float smoothstep(float t);

class SimpleNoise
{
public:
    void Init(uint32_t seed = 2001);
    float Eval(glm::vec2 p);

private:
    static const uint32_t c_MaxTableSize = 256;
    static const uint32_t c_MaxTableSizeMask = c_MaxTableSize - 1;
    float m_Rand[c_MaxTableSize];
    uint32_t m_PermutationTable[2 * c_MaxTableSize];
};

#endif