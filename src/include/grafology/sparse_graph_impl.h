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

        unsigned size() const { return _n_vertices; }

        unsigned capacity() const { return _n_max_vertices; }

        bool is_directed() const { return _is_directed; }

        weight_t operator()(unsigned i, unsigned j) const { 
             return _adjacency_list[i].get(j);
        }

        node_t add_vertex() {
            ++_n_vertices;
            _adjacency_list.emplace_back();
            return _n_vertices - 1;
        }

        /** 
         * @brief Batch add vertices
         * @remark This returns an iterator because when the removal of vertices will be implemented#
         * it will return first the "free" vertex slots, so indices won't be contiguous anymore.
        */
        generator<node_t> add_vertices(unsigned n) {
            for (unsigned i = 0; i < n; i++) {
                co_yield add_vertex();
            }
        }

        void set_edge(unsigned i, unsigned j, weight_t weight) {
            _adjacency_list[i].set(j, weight);
            if (!_is_directed) {
                _adjacency_list[j].set(i, weight);
            }
        }

        void set_edge(const edge_t& edge) {
            if (edge.weight == 0 && _adjacency_list[edge.start].get(edge.end) != 0) {
                _adjacency_list[edge.start].remove(edge.end);
                return;
            }
            _adjacency_list[edge.start].set(edge.end, edge.weight);
            if (!_is_directed) {
                _adjacency_list[edge.end].set(edge.start, edge.weight);
            }
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

        std::size_t degree(node_t node) const {
            return _adjacency_list[node].size();
        }

        std::size_t in_degree(node_t node) const {
            if (!is_directed()) {
                return degree(node);
            }
            std::size_t in_degree = 0;
            for (const auto& neighbors: _adjacency_list) {
                for (const auto& neighbor: neighbors) {
                    if (neighbor.node == node) {
                        ++in_degree;
                    }
                }
            }
            return in_degree;
        }

        generator<node_t> get_raw_neighbors(node_t node) const {
            for (const auto& edge : _adjacency_list[node]) {
                if (edge.node != node) {
                    co_yield edge.node;
                }
            }
        }

        generator<edge_t> get_neighbors(node_t node) const {
            for (const auto& edge : _adjacency_list[node])
            {
                if (edge.node != node) {
                    co_yield {.start = node, .end = edge.node, .weight = edge.weight};
                }
            }
        }

        generator<node_t> get_raw_in_neighbors(node_t node) const {
            for (unsigned i = 0; i < size(); ++i) {
                if (i == node) {
                    continue;
                }
                for (const auto& edge : _adjacency_list[i]) {
                    if (edge.node == node) {
                        co_yield i;
                    }
                }
            }
        }

        generator<edge_t> get_in_neighbors(node_t node) const {
            for (unsigned i = 0; i < size(); ++i) {
                if (i == node) {
                    continue;
                }
                for (const auto& edge : _adjacency_list[i]) {
                    if (edge.node == node) {
                        co_yield {.start = i, .end = node, .weight = edge.weight};
                    }
                }
            }
        }
   private:
        const bool _is_directed;
        const unsigned _n_max_vertices;
        unsigned _n_vertices;
        std::vector<FlatIndexMap> _adjacency_list;
    };

    static_assert(GraphImpl<SparseGraphImpl>);
} // namespace grafology
