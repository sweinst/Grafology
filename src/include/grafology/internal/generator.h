#pragma once

// this header allow to use std::generator with compilers which doesn't provide it
// VisualStudio 2022 17.12.3: in experimental 
// Apple's Clang 12.0.0: not available

#if __has_include(<generator>)
    #include <generator>
    template<typename _Ref, typename _Val = void, typename _Alloc = void>
    using generator = std::generator<_Ref, _Val, _Alloc>;
#else
#   if defined(_MSC_VER)
        // don't know when generator will be available in MSVC ?
        #include <experimental/generator>
        template <class _Ty, class _Alloc = std::allocator<char>>
        using generator = std::experimental::generator<_Ty, _Alloc>;
#   else
        #include "generator_impl.h"
        template<typename _Ref, typename _Val = void, typename _Alloc = void>
        using generator = std::generator<_Ref, _Val, _Alloc>;
#   endif 

#endif