
#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

struct GLFWwindow;

class OpenGLContext
{
public:
    OpenGLContext(GLFWwindow *window);
    void Init();
    void SwapBuffers();

private:
    GLFWwindow *m_WindowHandle;
};

#endif