#pragma once

#include <string>
#include <format>
#include <unordered_map>
#include "concepts_utils.h"

namespace grafology {
    /*!
    * @brief the requirements for identifying a Vertex
    * A VertexKey should allow to identify a vertex in a graph.
    *     
    * The requirements are:
    * - must be hashable using std::hash
    * - must be comparable (only equality required)
    * - must be printable using std::format
    */
    template <typename T>
    concept VertexKey = requires(T n, T u) {
        std::hash<T>{};
        CanEqualityCompare<T>;
        requires std::formattable<T, char>;
    };

} // namespace grafology
