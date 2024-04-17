#ifndef BASE_H
#define BASE_H

#include "pch/pch.h"

#define BIT(n) 1 << n

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
