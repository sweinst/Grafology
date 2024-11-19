#pragma once

#include <string>
#include <format>
#include <unordered_map>

namespace grafology {
    /*!
    * @brief the requirements for identifying a Vertex
    * A VertexKey should allow to identify a node in a graph.
    *     
    * The requirements are:
    * - must be hashable using std::hash
    * - must be comparable (only equality required)
    * - must be printable using std::format
    */
    template <typename T>
    concept VertexKey = requires(T n, T u) {
        std::hash<T>{};
        { n == u } noexcept -> std::convertible_to<bool>;
        requires std::formattable<T, char>;
    };

} // namespace grafology
