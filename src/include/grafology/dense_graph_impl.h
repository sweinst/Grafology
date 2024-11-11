#pragma once
#include "graph_impl.h"

namespace grafology {
    /**
     * @brief the dense graph implementation
     * 
     * The dense graph implementation is a graph where the edges are stored in a matrix.
     * Only weights are stored. If the weight is 0, then there is no edge.
     */
    class DenseGraphImpl {
    public:
         std::string label() const { return "hello"; }
    };

    static_assert(GraphImpl<DenseGraphImpl>);
} // namespace grafology
