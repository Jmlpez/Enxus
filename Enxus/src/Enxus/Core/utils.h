#ifndef UTILS_H
#define UTILS_H

#include "GLEW/glew.h"
#include "debugbreak/debugbreak.h"

// Force the program to stop if ASSERT fails
#define ASSERT(x) \
    if (!(x))     \
        debug_break();

#define LOG_LINE() \
    debugLine(__LINE__, __FILE__);

#define DEBUG

#ifdef DEBUG
#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x;
#endif

void GLClearError();
void debugLine(int line, const char *file);
bool GLLogCall(const char *function, const char *file, int line);

#endif
