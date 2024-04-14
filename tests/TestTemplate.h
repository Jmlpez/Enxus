
// WARNING: CHANGE THE GUARDS TO THE CORRECT NAME
#ifndef TEST_TEMPLATE_H
#define TEST_TEMPLATE_H

#include "Test.h"

namespace Test
{
    class TestTemplate : public Test
    {

    public:
        TestTemplate();
        ~TestTemplate();

        void OnUpdate(float deltaTime, Enxus::FreeCameraController *cameraController) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // private data
    };
}
#endif
