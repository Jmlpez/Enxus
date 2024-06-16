#include "pch.h"
#include "glm/glm.hpp"
#include "Math.h"

namespace Enxus::Math
{
    float Lerp(float a, float b, float t)
    {
        return a * t + b * (1.0f - t);
    }

    float InverseLerp(float edge0, float edge1, float t)
    {
        return edge0 == edge1 ? 1 : std::clamp((t - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    }

    float Smoothstep(float t)
    {
        return t * t * (3 - 2 * t);
    }
    float Blend(float t, float b)
    {
        float t3 = t * t * t;
        float bt = b - b * t;
        float bt3 = bt * bt * bt;
        return t3 / (t3 + bt3);
    }
    float BlendPow(float t, float a)
    {
        float tPowA = glm::pow(t, a);
        float compTPowA = glm::pow(1 - t, a);
        return tPowA / (tPowA + compTPowA);
    }
    float EaseInQuad(float t)
    {
        return t * t;
    }
    float EaseInCubic(float t)
    {
        return t * t * t;
    }
    float EaseInQuart(float t)
    {
        return t * t * t * t;
    }
    float EaseInQuint(float t)
    {
        return t * t * t * t * t;
    }
    float EaseInExpo(float t)
    {
        return t == 0 ? 0 : powf(1024, t - 1);
    }
    float EaseInCirc(float t)
    {
        return 1 - sqrtf(1 - t * t);
    }

    float EaseOutQuad(float t)
    {
        return 1 - EaseInQuad(1 - t);
    }
    float EaseOutCubic(float t)
    {
        return 1 - EaseInCubic(1 - t);
    }
    float EaseOutQuart(float t)
    {
        return 1 - EaseInQuart(1 - t);
    }
    float EaseOutQuint(float t)
    {
        return 1 - EaseInQuint(1 - t);
    }
    float EaseOutExpo(float t)
    {
        return t == 1 ? 1 : (1 - powf(1024, -t));
    }
    float EaseOutCirc(float t)
    {
        return sqrtf(1 - (t - 1) * (t - 1));
    }
}
