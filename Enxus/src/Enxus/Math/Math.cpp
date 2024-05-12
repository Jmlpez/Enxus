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
    float easeInQuad(float t)
    {
        return t * t;
    }
    float easeInCubic(float t)
    {
        return t * t * t;
    }
    float easeInQuart(float t)
    {
        return t * t * t * t;
    }
    float easeInQuint(float t)
    {
        return t * t * t * t * t;
    }
    float easeInExpo(float t)
    {
        return t == 0 ? 0 : powf(1024, t - 1);
    }
    float easeInCirc(float t)
    {
        return 1 - sqrtf(1 - t * t);
    }

    float easeOutQuad(float t)
    {
        return 1 - easeInQuad(1 - t);
    }
    float easeOutCubic(float t)
    {
        return 1 - easeInCubic(1 - t);
    }
    float easeOutQuart(float t)
    {
        return 1 - easeInQuart(1 - t);
    }
    float easeOutQuint(float t)
    {
        return 1 - easeInQuint(1 - t);
    }
    float easeOutExpo(float t)
    {
        return t == 1 ? 1 : (1 - powf(1024, -t));
    }
    float easeOutCirc(float t)
    {
        return sqrtf(1 - (t - 1) * (t - 1));
    }
}
