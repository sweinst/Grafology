#pragma once

#include "vertex.h"
#include "sparse_graph_impl.h"
#include "dense_graph_impl.h"
#include "bimap.h"

namespace grafology {
    template<VertexKey Vertex>
    struct Edge {
        Vertex start;
        Vertex end;
        weight_t weight;
    }; 

    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected>
    class Graph {
    public:
        using Edge = Edge<Vertex>;

        Graph(unsigned capacity): 
            _impl(capacity, 0, IsDirected),
            _vertex_map(capacity) {
        }

        void add_vertex(const Vertex& v) {
            _vertex_map.add_vertex(v);
            _impl.add_vertex();
        }

        unsigned size() const {
            return _impl.size();
        }

        unsigned capacity() const {
            return _impl.capacity();
        }

        template<input_iterator_value<Vertex> I, std::sentinel_for<I> S>
        void add_vertices(I first, S last) {
            for (auto it = first; it != last; ++it) {
                add_vertex(*it);
            }
        }

        template<input_range_value<Vertex> R>
        void add_vertices(R &&r) {
            add_vertices(std::begin(r), std::end(r));
        }

        void set_edge(const Vertex& start, const Vertex& end, weight_t weight) {
            auto i = _vertex_map.get_index(start);
            auto j = _vertex_map.get_index(end);
            _impl.set_edge(i, j, weight);
        }

        void set_edge(const Edge& edge) {
            set_edge(edge.start, edge.end, edge.weight);
        }

        template<input_iterator_value<Edge> I, std::sentinel_for<I> S>
        void set_edges(I first, S last) {
            for (auto it = first; it != last; ++it) {
                set_edge(*it);
            }
        }

        template<input_range_value<Edge> R>
        void set_edges(R &&r) {
            set_edges(std::begin(r), std::end(r));
        }

        unsigned degree(const Vertex& v) const {
            auto i = _vertex_map.get_index(v);
            return _impl.degree(i);
        }

        generator<Vertex> get_vertices() const {
            for (unsigned i = 0; i < size(); ++i) {
                co_yield _vertex_map.get_vertex(i);
            }
        }

        generator<Edge> get_neighbors(const Vertex& v) const {
            auto i = _vertex_map.get_index(v);
            for (const auto& edge : _impl.get_neighbors(i)) {
                co_yield {.start = v, .end = _vertex_map.get_vertex(edge.end), .weight = edge.weight};
            }
        }

    private:
        Impl _impl;
        BiMap<Vertex> _vertex_map;
    };

    template<VertexKey Vertex>
    using DirectedSparseGraph = Graph<SparseGraphImpl, Vertex, true>;

    template<VertexKey Vertex>
    using UndirectedSparseGraph = Graph<SparseGraphImpl, Vertex, false>;
    
    template<VertexKey Vertex>
    using DirectedDenseGraph = Graph<DenseGraphImpl, Vertex, true>;
    
    template<VertexKey Vertex>
    using UndirectedDenseGraph = Graph<DenseGraphImpl, Vertex, false>;

} // namespace grafology
