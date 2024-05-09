#include "pch.h"
#include "Math.h"

namespace Enxus::Math
{
    float lerp(float a, float b, float t)
    {
        return a * t + b * (1.0f - t);
    }
    float smoothstep(float t)
    {
        return t * t * (3 - 2 * t);
    }
    float blend(float t, float b)
    {
        float t3 = t * t * t;
        float bt = b - b * t;
        float bt3 = bt * bt * bt;
        return t3 / (t3 + bt3);
    }

}
