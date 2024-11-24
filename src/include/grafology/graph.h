#pragma once

#include "vertex.h"
#include "sparse_graph_impl.h"
#include "dense_graph_impl.h"
#include "bimap.h"

namespace grafology {
    /**
     * @brief The definition of an edge
     */
    template<VertexKey Vertex>
    struct EdgeDefinition {
        Vertex start;
        Vertex end;
        weight_t weight = 1;
    }; 

    /**
     * @brief The generic graph class
     */
    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected>
    class Graph {
    public:
        using Edge = EdgeDefinition<Vertex>;

        Graph(unsigned capacity): 
            _impl(capacity, 0, IsDirected),
            _vertex_map(capacity) {
        }

        vertex_t add_vertex(const Vertex& v) {
            _vertex_map.add_vertex(v);
            return _impl.add_vertex();
        }

        unsigned size() const {
            return _impl.size();
        }

        unsigned capacity() const {
            return _impl.capacity();
        }

        bool is_directed() const {
            return IsDirected;
        }

        const Impl& impl() const {
            return _impl;
        }

        vertex_t get_internal_index(const Vertex& v) const {
            return _vertex_map.get_index(v);
        }

        const Vertex& get_vertex_from_internal_index(vertex_t i) const {
            return _vertex_map.get_vertex(i);
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

        void set_edge(const Vertex& start, const Vertex& end, weight_t weight, bool create_vertices_if_missing = false) {
            auto i = _vertex_map.get_index(start);
            if (i == -1) {
                if (!create_vertices_if_missing)
                {
                    throw error(std::format("Vertex {}' not found", start));
                }
                i = add_vertex(start);
            }
            auto j = _vertex_map.get_index(end);
            if (j == -1) {
                if (!create_vertices_if_missing)
                {
                    throw error(std::format("Vertex {}' not found", end));
                }
                j = add_vertex(end);
            }
            _impl.set_edge(i, j, weight);
        }

        void set_edge(const Edge& edge, bool create_vertices_if_missing = false) {
            set_edge(edge.start, edge.end, edge.weight, create_vertices_if_missing);
        }

        template<input_iterator_value<Edge> I, std::sentinel_for<I> S>
        void set_edges(I first, S last, bool create_vertices_if_missing = false) {
            for (auto it = first; it != last; ++it) {
                set_edge(*it, create_vertices_if_missing);
            }
        }

        template<input_range_value<Edge> R>
        void set_edges(R &&r, bool create_vertices_if_missing = false) {
            set_edges(std::begin(r), std::end(r), create_vertices_if_missing);
        }

        unsigned degree(const Vertex& v) const {
            auto i = _vertex_map.get_index(v);
            return _impl.degree(i);
        }

        unsigned in_degree(const Vertex& v) const {
            auto i = _vertex_map.get_index(v);
            return _impl.in_degree(i);
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

        // TODO: check if "const Vertex &" can be used instead 
        generator<Vertex> get_raw_neighbors(const Vertex& v) const {
            auto i = _vertex_map.get_index(v);
            for (const auto& vertex : _impl.get_raw_neighbors(i)) {
                co_yield _vertex_map.get_vertex(vertex);
            }
        }

        generator<Edge> get_in_neighbors(const Vertex& v) const {
            auto i = _vertex_map.get_index(v);
            for (const auto& edge : _impl.get_in_neighbors(i)) {
                co_yield {.start = _vertex_map.get_vertex(edge.start), .end = v, .weight = edge.weight};
            }
        }

        // TODO: check if "const Vertex &" can be used instead 
        generator<Vertex> get_raw_in_neighbors(const Vertex& v) const {
            auto i = _vertex_map.get_index(v);
            for (const auto& vertex : _impl.get_raw_in_neighbors(i)) {
                co_yield _vertex_map.get_vertex(vertex);
            }
        }

        Graph invert() const {
            Graph inverted(capacity());
            inverted._impl = _impl.invert();
            inverted._vertex_map = _vertex_map;
            return inverted;
        }

    private:
        Impl _impl;
        BiMap<Vertex> _vertex_map;
    };

    /**
     * @brief A directed sparse graph
     */
    template<VertexKey Vertex>
    using DirectedSparseGraph = Graph<SparseGraphImpl, Vertex, true>;

    /**
     * @brief An undirected sparse graph
     */
    template<VertexKey Vertex>
    using UndirectedSparseGraph = Graph<SparseGraphImpl, Vertex, false>;
    
    /**
     * @brief A directed dense graph
     */
    template<VertexKey Vertex>
    using DirectedDenseGraph = Graph<DenseGraphImpl, Vertex, true>;
    
    /**
     * @brief A undirected dense graph
     */
    template<VertexKey Vertex>
    using UndirectedDenseGraph = Graph<DenseGraphImpl, Vertex, false>;

} // namespace grafology
