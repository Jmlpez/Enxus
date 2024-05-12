
#ifndef MATH_H
#define MATH_H

#include "Base.h"

namespace Enxus::Math
{

    float lerp(float a, float b, float t);
    float smoothstep(float t);
    /*
               t^a
    f(t) = --------------
            t^a - (b-t)^a
    This blend function is similar to smoothstep
    */
    float blend(float t, float b = 1.0f);

    // Ease-in curve animations

    float easeInQuad(float t);
    float easeInCubic(float t);
    float easeInQuart(float t);
    float easeInQuint(float t);
    float easeInExpo(float t);
    float easeInCirc(float t);

    // Ease-out curve animations
    float easeOutQuad(float t);
    float easeOutCubic(float t);
    float easeOutQuart(float t);
    float easeOutQuint(float t);
    float easeOutExpo(float t);
    float easeOutCirc(float t);

}
#endif
