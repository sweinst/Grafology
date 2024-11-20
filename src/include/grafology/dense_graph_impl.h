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
        // TODO: check bounds in debug builds
    public:
        DenseGraphImpl(unsigned n_max_vertices, unsigned n_vertices, bool is_directed) :
          _is_directed(is_directed),
          _n_max_vertices(n_max_vertices), 
          _n_vertices(n_vertices),
          _adjacency_matrix(_n_max_vertices * _n_max_vertices, 0) {
        }

        DenseGraphImpl(const DenseGraphImpl&) = default;
        DenseGraphImpl(DenseGraphImpl&&) = default;
        DenseGraphImpl& operator=(const DenseGraphImpl&) = default;
        DenseGraphImpl& operator=(DenseGraphImpl&&) = default;

        unsigned size() const { return _n_vertices; }

        unsigned capacity() const { return _n_max_vertices; }

        bool is_directed() const { return _is_directed; }

        // NB: VS2022 does not support the C++23 multidimensional subscript operator (i.e. a[i,j])
        weight_t operator()(unsigned i, unsigned j) const { 
            return _adjacency_matrix[i*_n_max_vertices+j];
        }

        node_t add_vertex() {
            std::memset(_adjacency_matrix.data() + _n_vertices*_n_max_vertices, 0, _n_max_vertices);
            for (node_t i = 0; i < _n_max_vertices; i++) {
                _adjacency_matrix[i*_n_max_vertices+_n_vertices] = 0;
            }            
            ++_n_vertices;
            return _n_vertices-1;
        }

        /** 
         * @brief Batch add vertices
         * @remark This returns an iterator because when the removal of vertices will be implemented#
         * it will return first the "free" vertex slots, so indices won't be contiguous anymore.
         * @remark In order to add the vertices, the generator must be iterated
        */
        generator<node_t> add_vertices(unsigned n) {
            for (unsigned i = 0; i < n; ++i) {
                co_yield add_vertex();
            }
        }

        void set_edge(unsigned i, unsigned j, weight_t weight) {
            _adjacency_matrix[i*_n_max_vertices+j] = weight;
            if (!_is_directed) {
                _adjacency_matrix[j*_n_max_vertices+i] = weight;
            }
        }

        void set_edge(const edge_t& edge) {
            _adjacency_matrix[edge.start*_n_max_vertices+edge.end] = edge.weight;
            if (!_is_directed) {
                _adjacency_matrix[edge.end*_n_max_vertices+edge.start] = edge.weight;
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
            std::size_t degree = 0;
            auto rowStart = _adjacency_matrix.data() + node*_n_max_vertices;
            for (node_t i = 0; i < _n_vertices; ++i) {
                if (*(rowStart+i) != 0) {
                    ++degree;
                }
            }
            return degree;
        }

        std::size_t in_degree(node_t node) const {
            std::size_t in_degree = 0;
            for (node_t i = 0; i < _n_vertices; ++i) {
                if (_adjacency_matrix[i*_n_max_vertices+node] != 0) {
                    ++in_degree;
                }
            }
            return in_degree;
        }

        generator<node_t> get_raw_neighbors(node_t node) const {
            auto rowStart = _adjacency_matrix.data() + node*_n_max_vertices;
            for (node_t i = 0; i < _n_vertices; ++i) {
                if (i != node) {
                    auto weight = *(rowStart+i);
                    if (weight != 0) {
                        co_yield i;
                    }
                }
            }
        }

        generator<edge_t> get_neighbors(node_t node) const {
            auto rowStart = _adjacency_matrix.data() + node*_n_max_vertices;
            for (node_t i = 0; i < _n_vertices; ++i) {
                if (i != node) {
                    auto weight = *(rowStart+i);
                    if (weight != 0) {
                        co_yield {.start = node, .end = i, .weight = weight};
                    }
                }
            }
        }

        generator<node_t> get_raw_in_neighbors(node_t node) const {
            for (node_t i = 0; i < _n_vertices; ++i) {
                if (i != node) {
                    auto weight = _adjacency_matrix[i*_n_max_vertices+node];
                    if (weight != 0) {
                        co_yield i;
                    }
                }
            }
        }
        
        generator<edge_t> get_in_neighbors(node_t node) const {
            for (node_t i = 0; i < _n_vertices; ++i) {
                if (i != node) {
                    auto weight = _adjacency_matrix[i*_n_max_vertices+node];
                    if (weight != 0) {
                        co_yield {.start = i, .end = node, .weight = weight};
                    }
                }
            }
        }

        DenseGraphImpl invert() const {
            DenseGraphImpl inverted(_n_max_vertices, _n_vertices, _is_directed);
            for (node_t i = 0; i < _n_vertices; ++i) {
                for (node_t j = 0; j < _n_vertices; ++j) {
                    inverted._adjacency_matrix[i*_n_max_vertices+j] = _adjacency_matrix[j*_n_max_vertices+i];
                }
            }
            return inverted;
        }

    private:
        bool _is_directed;
        unsigned _n_max_vertices;
        unsigned _n_vertices;
        std::vector<weight_t> _adjacency_matrix;
    };

    static_assert(GraphImpl<DenseGraphImpl>);

} // namespace grafology
