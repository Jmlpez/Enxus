#include "Window.h"
#include "utils.h"
#include "Application.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

namespace Enxus
{
    bool static s_GLFWInitialized = false;

    Scope<Window> Window::Create(const WindowProps &props)
    {
        return std::make_unique<Window>(props);
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

        // Set GLFW Callbacks
        glfwSetWindowSizeCallback(
            m_Window,
            [](GLFWwindow *window, int width, int height)
            {
                WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                // glViewport(0, 0, data.Width, data.Height);

                WindowResizeEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(
            m_Window,
            [](GLFWwindow *window)
            {
                WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(
            m_Window,
            [](GLFWwindow *window, int key, int scancode, int action, int mods)
            {
                WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                // KeyPress Event
                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleaseEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
                }
            });
        glfwSetMouseButtonCallback(
            m_Window,
            [](GLFWwindow *window, int button, int action, int mods)
            {
                WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
                // MousePressEvent

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }

                default:
                    break;
                }
            });
    }
    void Window::ShutDown()
    {
        glfwDestroyWindow(m_Window);
    }

    void Window::SetEventCallback(const EventCallbackFn &callback)
    {
        m_Data.EventCallback = callback;
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