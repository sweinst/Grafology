#pragma once
#include "../graph.h"
#include <random>

namespace grafology {
    /**
     * @brief A generator that allows the creation of large random graph which try to emulate graphs
     * found in real networks.
     * @see [R3MAT: A Rapid and Robust Graph
     * Generator](https://ieeexplore.ieee.org/document/9141251) for more details
     */
    class R3MatGeneratorBase {
       public:
        R3MatGeneratorBase(unsigned seed = 0) {
            if (seed != 0) {
                _rd.seed(seed);
            }
        }
        ~R3MatGeneratorBase() = default;

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
        generator<edge_t> generate_directed_edges();

        /**
         * @brief generates the edges of an undirected graph
         * 
         * @return a generator for the edges
         * @warning generate_degree_distribution must be called first
         */
        generator<edge_t> generate_undirected_edges();

        /**
         * @brief Get the vertex degrees distribution
         * @warning this is reset after calling generate_XXdirected_edges()
         */
        const std::vector<unsigned>& get_degrees() const { return _degrees; }

       private:

        edge_t choose_edge(
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

}  // namespace grafology
