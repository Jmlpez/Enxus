#ifndef TEST_H
#define TEST_H

#include "Camera.h"

namespace Test
{

    class Test
    {
    public:
        Test(){};
        virtual ~Test(){};

        virtual void OnUpdate(float deltaTime, FreeCameraController *cameraController)
        {
            (void)deltaTime;
            (void)cameraController;
        };
        virtual void OnRender(){};
        virtual void OnImGuiRender(){};
    };
}

#endif