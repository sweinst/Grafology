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
        
        template<input_iterator_edge I, std::sentinel_for<I> S>
        void set_edges(I first, S last) {
            for (auto it = first; it != last; ++it) {
                set_edge(it->start, it->end, it->weight);
                if (!_is_directed) {
                    set_edge(it->end, it->start, it->weight);
                }                
            }
        }

        template<input_range_edge R>
        void set_edges(R &&r) {
            set_edges(std::begin(r), std::end(r));
        }
    private:
        const bool _is_directed;
        const unsigned _n_max_vertices;
        const unsigned _n_vertices;
        std::vector<FlatIndexMap> _adjacency_list;
    };

    static_assert(GraphImpl<SparseGraphImpl>);
} // namespace grafology
