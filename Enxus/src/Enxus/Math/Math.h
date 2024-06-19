
#ifndef MATH_H
#define MATH_H

#include "Base.h"

namespace Enxus::Math
{

    float Lerp(float a, float b, float t);
    float InverseLerp(float edge0, float edge1, float t);
    float Smoothstep(float edge0, float edge1, float t);
    float Smoothstep(float t);
    /*
               t^a
    f(t) = --------------
            t^a - (b-t)^a
    This blend function is similar to smoothstep
    In this case 'a' is fixed to 3.0
    */
    float Blend(float t, float b = 1.0f);

    /*
    Similar to blend but 'a' is not fixed
    and pow is used instead
    */
    float BlendAlpha(float t, float a);
    /*
    Similar to BlendAlpha but 'a' control
    the blending factor  and 'b'
    */
    float BlendAlphaBeta(float t, float a, float b);

    // Ease-in curve animations

    float EaseInQuad(float t);
    float EaseInCubic(float t);
    float EaseInQuart(float t);
    float EaseInQuint(float t);
    float EaseInExpo(float t);
    float EaseInCirc(float t);

    // Ease-out curve animations
    float EaseOutQuad(float t);
    float EaseOutCubic(float t);
    float EaseOutQuart(float t);
    float EaseOutQuint(float t);
    float EaseOutExpo(float t);
    float EaseOutCirc(float t);

}
#endif
