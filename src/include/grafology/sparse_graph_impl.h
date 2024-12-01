#pragma once
#include "flat_index_map.h"

namespace grafology {
    /**
     * @brief A sparse graph implementation which use an adjacency list
     * @warning Once built, the graph capacity cannot be changed.
     */
    class SparseGraphImpl {
        // TODO: check bounds in debug builds
    public:
        SparseGraphImpl(unsigned n_max_vertices, unsigned n_vertices, bool is_directed) :
          _is_directed(is_directed),
          _n_max_vertices(n_max_vertices), 
          _n_vertices(n_vertices),
          _adjacency_list(_n_max_vertices) {
            _adjacency_list.reserve(_n_max_vertices);
        }

        SparseGraphImpl(const SparseGraphImpl&) = default;
        SparseGraphImpl(SparseGraphImpl&&) = default;
        SparseGraphImpl& operator=(const SparseGraphImpl&) = default;
        SparseGraphImpl& operator=(SparseGraphImpl&&) = default;
        bool operator==(const SparseGraphImpl& other) const = default;
        bool operator!=(const SparseGraphImpl& other) const = default;

        unsigned size() const { return _n_vertices; }

        unsigned capacity() const { return _n_max_vertices; }

        bool is_directed() const { return _is_directed; }

        weight_t operator()(vertex_t i, vertex_t j) const { 
             return _adjacency_list[i].get(j);
        }

        bool has_edge(vertex_t i, vertex_t j) const { 
             return _adjacency_list[i].get(j) != 0;
        }

        vertex_t add_vertex() {
            ++_n_vertices;
            return _n_vertices - 1;
        }

        /** 
         * @brief Batch add vertices
         * @remark This returns an iterator because when the removal of vertices will be implemented#
         * it will return first the "free" vertex slots, so indices won't be contiguous anymore.
        */
        generator<vertex_t> add_vertices(unsigned n) {
            for (unsigned i = 0; i < n; i++) {
                co_yield add_vertex();
            }
        }

        void set_edge(vertex_t start, vertex_t end, weight_t weight) {
            if (weight == 0 && _adjacency_list[start].get(end) != 0) {
                _adjacency_list[start].remove(end);
                return;
            }
            _adjacency_list[start].set(end, weight);
            if (!_is_directed) {
                _adjacency_list[end].set(start, weight);
            }
        }

        void set_edge(const edge_t& edge) {
            set_edge(edge.start, edge.end, edge.weight);
        }
        
        template<input_iterator_value<edge_t> I, std::sentinel_for<I> S>
        void set_edges(I first, S last) {
            for (auto it = first; it != last; ++it) {
                set_edge(it->start, it->end, it->weight);
                if (!_is_directed) {
                    set_edge(it->end, it->start, it->weight);
                }                
            }
        }

        template<input_range_value<edge_t> R>
        void set_edges(R &&r) {
            set_edges(std::begin(r), std::end(r));
        }

        std::size_t degree(vertex_t vertex) const {
            return _adjacency_list[vertex].size();
        }

        std::size_t in_degree(vertex_t vertex) const {
            if (!is_directed()) {
                return degree(vertex);
            }
            std::size_t in_degree = 0;
            for (const auto& neighbors: _adjacency_list) {
                for (const auto& neighbor: neighbors) {
                    if (neighbor.vertex == vertex) {
                        ++in_degree;
                    }
                }
            }
            return in_degree;
        }

        generator<vertex_t> get_raw_neighbors(vertex_t vertex) const {
            for (const auto& edge : _adjacency_list[vertex]) {
                if (edge.vertex != vertex) {
                    co_yield edge.vertex;
                }
            }
        }

        generator<edge_t> get_neighbors(vertex_t vertex) const {
            for (const auto& edge : _adjacency_list[vertex])
            {
                if (edge.vertex != vertex) {
                    co_yield {.start = vertex, .end = edge.vertex, .weight = edge.weight};
                }
            }
        }

        generator<vertex_t> get_raw_in_neighbors(vertex_t vertex) const {
            for (unsigned i = 0; i < size(); ++i) {
                if (i == vertex) {
                    continue;
                }
                for (const auto& edge : _adjacency_list[i]) {
                    if (edge.vertex == vertex) {
                        co_yield i;
                    }
                }
            }
        }

        generator<edge_t> get_in_neighbors(vertex_t vertex) const {
            for (unsigned i = 0; i < size(); ++i) {
                if (i == vertex) {
                    continue;
                }
                for (const auto& edge : _adjacency_list[i]) {
                    if (edge.vertex == vertex) {
                        co_yield {.start = i, .end = vertex, .weight = edge.weight};
                    }
                }
            }
        }

        SparseGraphImpl invert() const {
            SparseGraphImpl inverted(_n_max_vertices, _n_vertices, _is_directed);
            for (unsigned i = 0; i < _n_vertices; i++) {
                for (const auto& edge : _adjacency_list[i]) {
                    inverted.set_edge(edge.vertex, i, edge.weight);
                }
            }
            return inverted;
        }

        generator<edge_t> get_all_edges() const {
            if (is_directed()) {
                for (unsigned i = 0; i < _n_vertices; i++) {
                    for (const auto& edge : _adjacency_list[i]) {
                        co_yield {.start = i, .end = edge.vertex, .weight = edge.weight};
                    }
                }
            }
            else {
                for (unsigned i = 0; i < _n_vertices; i++) {
                    for (const auto& edge : _adjacency_list[i]) {
                        // we need to avoid duplicates
                        if (i <= edge.vertex) {
                            co_yield {.start = i, .end = edge.vertex, .weight = edge.weight};
                        }
                    }
                }
            }
        }

   private:
        bool _is_directed;
        unsigned _n_max_vertices;
        unsigned _n_vertices;
        std::vector<FlatIndexMap> _adjacency_list;
    };

    static_assert(GraphImpl<SparseGraphImpl>);
} // namespace grafology
