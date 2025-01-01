#pragma once
#include "graph_impl.h"
#include <vector>

namespace grafology {
    /**
     * @brief A dense graph implementation which use an adjacency matrix
     * @details The adjacency matrix is a matrix where the rows and columns are the vertices of the graph.
     * The value of the cells are the weight of the edges.
     * If the weight is 0, then there is no edge.
     * @warning Once built, the graph capacity cannot be changed.
     */
    class DenseGraphImpl {
    public:
        DenseGraphImpl(unsigned n_max_vertices, unsigned n_vertices, bool is_directed) :
          _is_directed(is_directed),
          _n_max_vertices(n_max_vertices), 
          _n_vertices(n_vertices),
          _adjacency_matrix(_n_max_vertices * _n_max_vertices, 0) {
            assert(n_vertices <= n_max_vertices);
        }

        DenseGraphImpl(const DenseGraphImpl&) = default;
        DenseGraphImpl(DenseGraphImpl&&) = default;
        DenseGraphImpl& operator=(const DenseGraphImpl&) = default;
        DenseGraphImpl& operator=(DenseGraphImpl&&) = default;
        bool operator==(const DenseGraphImpl& other) const = default;
        bool operator!=(const DenseGraphImpl& other) const = default;

        unsigned size() const { return _n_vertices; }

        unsigned capacity() const { return _n_max_vertices; }

        bool is_directed() const { return _is_directed; }

        weight_t operator()(unsigned i, unsigned j) const { 
            assert(i < _n_vertices && j < _n_vertices);
            return _adjacency_matrix[i*_n_max_vertices+j];
        }

        bool has_edge(vertex_t i, vertex_t j) const { 
            assert(i < _n_vertices && j < _n_vertices);
            return _adjacency_matrix[i*_n_max_vertices+j] != 0;
        }
        
        weight_t weight(vertex_t i, vertex_t j) const { 
            assert(i < _n_vertices && j < _n_vertices);
            return _adjacency_matrix[i*_n_max_vertices+j];
        }
        
        vertex_t add_vertex() {
            assert(_n_vertices < _n_max_vertices);
            std::memset(_adjacency_matrix.data() + _n_vertices*_n_max_vertices, 0, _n_max_vertices);
            for (vertex_t i = 0; i < _n_max_vertices; i++) {
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
        generator<vertex_t> add_vertices(unsigned n) {
            assert(_n_vertices + n <= _n_max_vertices);
            for (unsigned i = 0; i < n; ++i) {
                co_yield add_vertex();
            }
        }

        void set_edge(vertex_t i, vertex_t j, weight_t weight) {
            assert(i < _n_vertices && j < _n_vertices);
            _adjacency_matrix[i*_n_max_vertices+j] = weight;
            if (!_is_directed) {
                _adjacency_matrix[j*_n_max_vertices+i] = weight;
            }
        }

        void set_edge(const edge_t& edge) {
            assert(edge.start < _n_vertices && edge.end < _n_vertices);
            _adjacency_matrix[edge.start*_n_max_vertices+edge.end] = edge.weight;
            if (!_is_directed) {
                _adjacency_matrix[edge.end*_n_max_vertices+edge.start] = edge.weight;
            }
        }

        template<input_iterator_value<edge_t> I, std::sentinel_for<I> S>
        void set_edges(I first, S last) {
            for (auto it = first; it != last; ++it) {
                assert(it->start < _n_vertices && it->end < _n_vertices);
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
            assert(vertex < _n_vertices);
            std::size_t degree = 0;
            auto rowStart = _adjacency_matrix.data() + vertex*_n_max_vertices;
            for (vertex_t i = 0; i < _n_vertices; ++i) {
                if (*(rowStart+i) != 0) {
                    ++degree;
                }
            }
            return degree;
        }

        std::size_t in_degree(vertex_t vertex) const {
            assert(vertex < _n_vertices);
            std::size_t in_degree = 0;
            for (vertex_t i = 0; i < _n_vertices; ++i) {
                if (_adjacency_matrix[i*_n_max_vertices+vertex] != 0) {
                    ++in_degree;
                }
            }
            return in_degree;
        }

        generator<vertex_t> get_raw_neighbors(vertex_t vertex) const {
            assert(vertex < _n_vertices);
            auto rowStart = _adjacency_matrix.data() + vertex*_n_max_vertices;
            for (vertex_t i = 0; i < _n_vertices; ++i) {
                if (i != vertex) {
                    auto weight = *(rowStart+i);
                    if (weight != 0) {
                        co_yield i;
                    }
                }
            }
        }

        generator<edge_t> get_neighbors(vertex_t vertex) const {
            assert(vertex < _n_vertices);
            auto rowStart = _adjacency_matrix.data() + vertex*_n_max_vertices;
            for (vertex_t i = 0; i < _n_vertices; ++i) {
                if (i != vertex) {
                    auto weight = *(rowStart+i);
                    if (weight != 0) {
                        co_yield {.start = vertex, .end = i, .weight = weight};
                    }
                }
            }
        }

        generator<vertex_t> get_raw_in_neighbors(vertex_t vertex) const {
            assert(vertex < _n_vertices);
            for (vertex_t i = 0; i < _n_vertices; ++i) {
                if (i != vertex) {
                    auto weight = _adjacency_matrix[i*_n_max_vertices+vertex];
                    if (weight != 0) {
                        co_yield i;
                    }
                }
            }
        }
        
        generator<edge_t> get_in_neighbors(vertex_t vertex) const {
            assert(vertex < _n_vertices);
            for (vertex_t i = 0; i < _n_vertices; ++i) {
                if (i != vertex) {
                    auto weight = _adjacency_matrix[i*_n_max_vertices+vertex];
                    if (weight != 0) {
                        co_yield {.start = i, .end = vertex, .weight = weight};
                    }
                }
            }
        }

        DenseGraphImpl invert() const {
            DenseGraphImpl inverted(_n_max_vertices, _n_vertices, _is_directed);
            for (vertex_t i = 0; i < _n_vertices; ++i) {
                for (vertex_t j = 0; j < _n_vertices; ++j) {
                    inverted._adjacency_matrix[i*_n_max_vertices+j] = _adjacency_matrix[j*_n_max_vertices+i];
                }
            }
            return inverted;
        }

        generator<edge_t> get_all_edges() const {
            if (is_directed()) {
                for (vertex_t i = 0; i < _n_vertices; ++i) {
                    for (vertex_t j = 0; j < _n_vertices; ++j) {
                        if (_adjacency_matrix[i*_n_max_vertices+j] != 0) {
                            co_yield {.start = i, .end = j, .weight = _adjacency_matrix[i*_n_max_vertices+j]};
                        }
                    }
                }
            } else {
                for (vertex_t i = 0; i < _n_vertices; ++i) {
                    for (vertex_t j = i; j < _n_vertices; ++j) {
                        if (_adjacency_matrix[i*_n_max_vertices+j] != 0) {
                            co_yield {.start = i, .end = j, .weight = _adjacency_matrix[i*_n_max_vertices+j]};
                        }
                    }
                }
            }
        }


    private:
        bool _is_directed;
        unsigned _n_max_vertices;
        unsigned _n_vertices;
        std::vector<weight_t> _adjacency_matrix;
    };

    static_assert(GraphImpl<DenseGraphImpl>);

} // namespace grafology
