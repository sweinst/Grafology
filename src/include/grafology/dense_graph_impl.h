#pragma once
#include "graph_impl.h"
#include <vector>

namespace grafology {
    /**
     * @brief A dense graph implementation which use an adjacency matrix
     * @details The adjacency matrix is a matrix where the rows and columns are the nodes of the graph.
     * The value of the cells are the weight of the edges.
     * If the weight is 0, then there is no edge.
     * @warning Once built, the graph capacity cannot be changed.
     */
    class DenseGraphImpl {
        // TODO: check bounds
    public:
        DenseGraphImpl(unsigned n_max_vertices, unsigned n_vertices, bool is_directed) :
          _is_directed(is_directed),
          _n_max_vertices(n_max_vertices), 
          _n_vertices(n_vertices),
          _matrix(_n_max_vertices * _n_max_vertices, 0) {
        }
        // NB: VS2022 does not support the C++23 multidimensional subscript operator (i.e. a[i,j])
        weight_t& operator()(unsigned i, unsigned j) {
            return _matrix[i*_n_max_vertices+j];
        }

        weight_t operator()(unsigned i, unsigned j) const { 
            return _matrix[i*_n_max_vertices+j];
        }

        void set_edge(unsigned i, unsigned j, weight_t weight) {
            (*this)(i, j) = weight;
            if (!_is_directed) {
                (*this)(j, i) = weight;
            }
        }

        void set_edge(const edge_t* edge) {
            (*this)(edge->start, edge->end) = edge->weight;
            if (!_is_directed) {
                (*this)(edge->end, edge->start) = edge->weight;
            }
        }

        template<input_iterator_edge I, std::sentinel_for<I> S>
        void set_edges(I first, S last) {
            for (auto it = first; it != last; ++it) {
                (*this)(it->start, it->end) = it->weight;
                if (!_is_directed) {
                    (*this)(it->end, it->start) = it->weight;
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
        std::vector<weight_t> _matrix;
    };

//  static_assert(GraphImpl<DenseGraphImpl>);

} // namespace grafology
