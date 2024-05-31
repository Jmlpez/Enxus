#ifndef TEST_H
#define TEST_H

#include "Enxus.h"

namespace OpenGLTest
{
    class Test
    {
    public:
        Test(){};
        virtual ~Test(){};
        virtual void OnUpdate(Enxus::Camera &camera) { (void)camera; }
        virtual void OnShadowPass() {};
        virtual void OnImGuiRender() {}
    };
}

#endif