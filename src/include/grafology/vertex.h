#pragma once

#include <string>
#include <format>
#include <unordered_map>

namespace grafology {
/*!
 * @brief the requirements for a Node
 * The requirements are:
 * - must be hashable using std::hash
 * - must be comparable (only equality required)
 * - must be printable using std::format
 */
template <typename T>
concept Vertex = requires(T n) {
    // must be "hashable"
    std::hash<T>{};
    // must be "comparable" without having to deal with exceptions
    { n == n } noexcept -> std::convertible_to<bool>;
    // we should be able to "print" it
    requires std::formattable<T, char>;
};

} // namespace grafology
