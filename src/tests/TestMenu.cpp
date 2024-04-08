#include "TestMenu.h"
#include "imgui.h"

namespace Test
{
    TestMenu::TestMenu(Test *&currentTestPointer) : m_CurrentTest(currentTestPointer)
    {
    }

    TestMenu::~TestMenu()
    {
    }

    void TestMenu::OnImGuiRender()
    {

        for (auto &[label, createTest] : m_Tests)
        {
            if (ImGui::Button(label.c_str()))
            {
                m_CurrentTest = createTest();
            }
        }
    }
}
