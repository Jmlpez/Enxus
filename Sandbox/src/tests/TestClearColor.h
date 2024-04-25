#ifndef TEST_CLEAR_COLOR_H
#define TEST_CLEAR_COLOR_H

#include "Test.h"
#include "Enxus.h"

namespace OpenGLTest
{

    class TestClearColor : public Test
    {

    public:
        TestClearColor();
        ~TestClearColor();

        void OnUpdate(Enxus::Camera &camera) override;
        void OnImGuiRender() override;

    private:
        float m_ClearColor[4];
    };
}
#endif
