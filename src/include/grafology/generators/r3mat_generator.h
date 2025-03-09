#pragma once
#include "../graph.h"
#include <random>

namespace grafology {
    struct base_edge_t {
        unsigned start;
        unsigned end;

        template<typename weight_t> 
        operator edge_t<weight_t>() const {
            return {start, end, 1};
        }
    };

    /**
     * @brief A generator that allows the creation of large random graph which try to emulate graphs
     * found in real networks.
     * @see [R3MAT: A Rapid and Robust Graph Generator](https://ieeexplore.ieee.org/document/9141251) for more details
     */
    class R3MatGenerator {
       public:
        R3MatGenerator(unsigned seed = 0) {
            if (seed != 0) {
                _rd.seed(seed);
            }
        }
        ~R3MatGenerator() = default;

        /**]
         * @brief generates the degree distribution of the graph
         */
        void generate_degree_distribution(bool is_directed, unsigned n_vertices);

        /**
         * @brief generates the edges of a directed graph
         * 
         * @return a generator for the edges
         * @warning generate_degree_distribution must be called first
         */
        generator<base_edge_t> generate_directed_edges();

        /**
         * @brief generates the edges of an undirected graph
         * 
         * @param preserve_distribution if true, the array giving the distribution of the degrees is preserved
         * @return a generator for the edges
         * @warning generate_degree_distribution must be called first
         */
        generator<base_edge_t> generate_undirected_edges(bool preserve_distribution = false);

        /**
         * @brief Get the vertex degrees distribution
         * @warning this is reset after calling generate_XXdirected_edges()
         */
        const std::vector<unsigned>& get_degrees() const { return _degrees; }

       private:

        base_edge_t choose_edge(
            unsigned x1,
            unsigned y1,
            unsigned xn,
            unsigned yn,
            double a,
            double b,
            double c,
            double d
        );

        std::vector<unsigned> _degrees;
        std::mt19937 _rd;
        std::uniform_real_distribution<double> _rnd_gen{0., 1.};
        double _offset1;
        double _offset2;
        double _offset3;
        double _offset4;
    };

    template <typename Impl>
    requires GraphImpl<Impl, typename Impl::weight_lt>
    Impl generate_r3mat_graph(unsigned n_max_vertices, unsigned n_vertices, bool is_directed, unsigned seed = 0) {
        R3MatGenerator gen(seed);
        gen.generate_degree_distribution(is_directed, n_vertices);
        auto edges = is_directed ? gen.generate_directed_edges() : gen.generate_undirected_edges();
        Impl g(n_max_vertices, n_vertices, is_directed);
        for (auto e : edges) {
            g.set_edge(e);
        }
        return g;
    }

    template <typename weight_t>
    inline SparseGraphImpl<weight_t> generate_r3mat_sparse_graph(unsigned n_max_vertices, unsigned n_vertices, bool is_directed, unsigned seed = 0) {
        return generate_r3mat_graph<SparseGraphImpl<weight_t>>(n_max_vertices, n_vertices, is_directed, seed);
    }

    template <typename weight_t>
    inline DenseGraphImpl<weight_t> generate_r3mat_dense_graph(unsigned n_max_vertices, unsigned n_vertices, bool is_directed, unsigned seed = 0) {
        return generate_r3mat_graph<DenseGraphImpl<weight_t>>(n_max_vertices, n_vertices, is_directed, seed);
    }
}  // namespace grafology
