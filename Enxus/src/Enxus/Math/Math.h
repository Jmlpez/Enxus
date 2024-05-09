
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

}
#endif
