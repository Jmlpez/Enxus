#include "Application.h"

namespace Enxus
{
    Application::Application()
    {
        m_Window = CreateScope<Window>(Window::Create());
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }
}