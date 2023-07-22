#pragma once

#include "core/ftl/type_traits.h"

namespace flavo::utils
{
    template<auto Start, auto End, auto Inc, typename F>
    constexpr void ConstexprForLoop(F&& func)
    {
        if constexpr (Start < End)
        {
            func(ftl::integral_constant<decltype(Start), Start>());
            ConstexprForLoop<Start + Inc, End, Inc>(func);
        }
    }

    template<typename F, typename ...Args>
    constexpr void ConstexprForLoop(F&& func, Args&&... args)
    {
        (func(ftl::forward<Args>(args)), ...);
    }
}
