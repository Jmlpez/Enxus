#ifndef UTILS_H
#define UTILS_H

#include <GLEW/glew.h>

#include "debugbreak.h"
#include <memory> // to handle smart pointers
#include <stdint.h>
#include <iostream>

// Force the program to stop if ASSERT fails
#define ASSERT(x) \
    if (!(x))     \
        debug_break();

#define LOG_LINE() \
    debugLine(__LINE__, __FILE__);

#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
void debugLine(int line, const char *file);
bool GLLogCall(const char *function, const char *file, int line);

/*
It's more semantic to use Scope and Ref, to refer the unique and shared pointers of my Engine. Idea from The Cherno :)
In the future, I can make my own class or my own reference count system.
*/
namespace Enxus
{

    template <typename T>
    using Scope = std::unique_ptr<T>;

    // To create unique and shared pointer using custom functions with zero overhead
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif
