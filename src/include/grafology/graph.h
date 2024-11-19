#pragma once

#include "vertex.h"
#include "sparse_graph_impl.h"
#include "dense_graph_impl.h"

namespace grafology {
    template<GraphImpl Impl, Vertex Vertex, bool IsDirected>
    class Graph {
    public:
        Graph(unsigned capacity): 
            _impl(capacity, 0, IsDirected) {
        }
    private:
        Impl _impl;
    };

    template<Vertex Vertex>
    using DirectedSparseGraph = Graph<SparseGraphImpl, Vertex, true>;

    template<Vertex Vertex>
    using UndirectedSparseGraph = Graph<SparseGraphImpl, Vertex, false>;
    
    template<Vertex Vertex>
    using DirectedDenseGraph = Graph<DenseGraphImpl, Vertex, true>;
    
    template<Vertex Vertex>
    using UndirectedDenseGraph = Graph<DenseGraphImpl, Vertex, false>;

} // namespace grafology
