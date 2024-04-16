#include "Window.h"
#include "utils.h"

namespace Enxus
{
    bool static s_GLFWInitialized = false;

    Window *Window::Create(const WindowProps &props)
    {
        return new Window(props);
    }

    Window::Window(const WindowProps &props)
    {
        Init(props);
    }

    Window::~Window()
    {
        ShutDown();
    }

    void Window::Init(const WindowProps &props)
    {
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.Title = props.Title;

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            if (!success)
            {
                std::cout << "[GLFW Error]: Could not initialize GLFW :(" << std::endl;
                ASSERT(false);
            }
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            std::cout << "[GLFW Error]: Could not create a window :(" << std::endl;
            ASSERT(false);
        }
        glfwMakeContextCurrent(m_Window);
        if (glewInit() != GLEW_OK)
        {
            std::cout << "[GLEW Error]: failed to initialize glew :(" << std::endl;
            ASSERT(false);
        }

        glfwSetWindowUserPointer(m_Window, &m_Data);

        SetVSync(true);
    }
    void Window::ShutDown()
    {
        glfwDestroyWindow(m_Window);
    }

    void Window::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::SetVSync(bool enabled)
    {
        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
        m_Data.VSync = enabled;
    }

}