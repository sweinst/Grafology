#pragma once
#include <concepts>

namespace grafology {
    // to get around the fact that an equality operator is implicitly defined when a spaceship operator is defined
    // so we can compare  them if the equality operator or the spaceship operator is defined
    template <typename T>
    concept CanEqualityCompare =  
        requires(T a, T b) {
            { a == b } -> std::convertible_to<bool>;
        }
        ||
        requires(T a, T b) {
            { a <=> b } -> std::convertible_to<bool>;
        }
    ;
} // namespace grafology