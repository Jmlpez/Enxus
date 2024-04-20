#include "pch.h"
#include "utils.h"
#include "OpenGLContext.h"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

OpenGLContext::OpenGLContext(GLFWwindow *window)
    : m_WindowHandle(window)
{
}
void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "[GLEW Error]: failed to initialize glew :(" << std::endl;
        ASSERT(false);
    }
#if 0
    std::cout << "OpenGL Info:" << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
#endif
}
void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
}