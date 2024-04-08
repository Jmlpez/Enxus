#ifndef TEST_MENU_H
#define TEST_MENU_H

#include "Test.h"
#include <iostream>
#include <vector>
#include <functional>

namespace Test
{

    class TestMenu : public Test
    {
    public:
        TestMenu(Test *&currentTestPointer);
        ~TestMenu() override;

        void OnImGuiRender() override;

        // implementing this method here to avoid common problems with templates
        template <typename T>
        void RegisterTest(const std::string &testName)
        {
            std::cout << "Registering Test: " << testName << std::endl;
            auto createTest = [&]()
            { return new T(); };
            m_Tests.push_back({testName, createTest});
        }

    private:
        Test *&m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test *()>>> m_Tests;
    };
}
#endif
