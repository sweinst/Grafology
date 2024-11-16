#pragma once
#include "flat_index_map.h"

namespace grafology {
    class SparseGraphImpl {
        // TODO: check bounds
    public:
        SparseGraphImpl(unsigned n_max_vertices, unsigned n_vertices, bool is_directed) :
          _is_directed(is_directed),
          _n_max_vertices(n_max_vertices), 
          _n_vertices(n_vertices),
          _adjacency_list(_n_max_vertices) {
            _adjacency_list.reserve(_n_max_vertices);
        }

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
            _adjacency_list[edge.start].set(edge.start, edge.weight);
            if (!_is_directed) {
                _adjacency_list[edge.start].set(edge.start, edge.weight);
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
    private:
        const bool _is_directed;
        const unsigned _n_max_vertices;
        unsigned _n_vertices;
        std::vector<FlatIndexMap> _adjacency_list;
    };

    static_assert(GraphImpl<SparseGraphImpl>);
} // namespace grafology
