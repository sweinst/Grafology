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

        bool operator ==(const EdgeDefinition&) const = default;
    };

} // namespace grafology

namespace std {
    /**
     *  @brief Specialization of std::hash for EdgeDefinition
     */
    template <typename Vertex>
        requires grafology::VertexKey<Vertex>
    struct hash<grafology::EdgeDefinition<Vertex>> {
        std::size_t operator() (const grafology::EdgeDefinition<Vertex>& e) const {
            auto h1 = std::hash<Vertex>{}(e.start);
            auto h2 = std::hash<Vertex>{}(e.end);
            return (h1 ^ (h2 << 1));
        }
    };

    /** 
     * @brief Specialization of std::format for EdgeDefinition<Vertex>
     */
    template <grafology::VertexKey Vertex>
    struct formatter<grafology::EdgeDefinition<Vertex>, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            return ctx.begin();
        }

        template <class FormatContext>
        auto format(const grafology::EdgeDefinition<Vertex>& v, FormatContext& ctx) const {
            return std::format_to(ctx.out(), "[start:{},end:{},weight:{}]", v.start, v.end, v.weight);
        }
    };
} // namespace std

namespace grafology {
    /**
     * @brief The generic graph class
     * @tparam Impl The graph implementation 
     * @tparam Vertex The class used for identifying vertices
     * @tparam IsDirected Whether the graph is directed or not
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

        static constexpr bool is_directed() {
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
                    throw error("Vertex '{}' not found", start);
                }
                i = add_vertex(start);
            }
            auto j = _vertex_map.get_index(end);
            if (j == -1) {
                if (!create_vertices_if_missing)
                {
                    throw error("Vertex '{}' not found", end);
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

        generator<Edge> get_all_edges() const {
            for (const auto& edge : _impl.get_all_edges()) {
                co_yield {.start = _vertex_map.get_vertex(edge.start), .end = _vertex_map.get_vertex(edge.end), .weight = edge.weight};
            }
        }

    private:
        Impl _impl;
        BiMap<Vertex> _vertex_map;
    };

    /**
     * @brief A directed sparse graph
     * @tparam Vertex The class used for identifying vertices
     */
    template<VertexKey Vertex>
    using DirectedSparseGraph = Graph<SparseGraphImpl, Vertex, true>;

    /**
     * @brief An undirected sparse graph
     * @tparam Vertex The class used for identifying vertices
     */
    template<VertexKey Vertex>
    using UndirectedSparseGraph = Graph<SparseGraphImpl, Vertex, false>;
    
    /**
     * @brief A directed dense graph
     * @tparam Vertex The class used for identifying vertices
     */
    template<VertexKey Vertex>
    using DirectedDenseGraph = Graph<DenseGraphImpl, Vertex, true>;
    
    /**
     * @brief An undirected dense graph
     * @tparam Vertex The class used for identifying vertices
     */
    template<VertexKey Vertex>
    using UndirectedDenseGraph = Graph<DenseGraphImpl, Vertex, false>;

} // namespace grafology
