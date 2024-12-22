#pragma once

// this header allow to use std::generator with compilers which doesn't provide it
// VisualStudio 2022 17.12.3: in experimental 
// g++ > 14.0 is required
// Clang > 19.0 is required

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
        static_assert(false, "std::generator is not available");
#   endif 
#endif
