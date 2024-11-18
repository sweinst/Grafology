#include "test_vertex.h"
#include <catch2/catch_template_test_macros.hpp>
#include <grafology/dense_graph_impl.h>
#include <grafology/sparse_graph_impl.h>
#include <iostream>

TEMPLATE_TEST_CASE("Graph implementations", "[graph-impl]", g::DenseGraphImpl, g::SparseGraphImpl)
{
    constexpr unsigned max_vertices = 20;
    unsigned n_vertices = 7;
    const std::vector<g::edge_t> edges_init {
        {0, 1, 1},
        {0, 2, 2},
        {2, 3, 5},
        {2, 4, 6},
        {2, 5, 7},
        {3, 1, 4},
        {5, 1, 6},
    };

    SECTION("Directed graphs")
    {   
        std::vector<unsigned> degrees { 2, 0, 3, 1, 0, 1 };

        TestType g(max_vertices, n_vertices, true);
        g.set_edges(edges_init);
        for (const auto [idx, degree]: std::views::enumerate(degrees))
        {
            CAPTURE(idx, degree, g.degree(idx));
            CHECK(g.degree(idx) == degree);
        }
    }    

    SECTION("Undirected graphs")
    {   
        std::vector<unsigned> degrees { 2, 3, 4, 2, 1, 2 };

        TestType g(max_vertices, n_vertices, false);
        g.set_edges(edges_init);
        for (const auto [idx, degree]: std::views::enumerate(degrees))
        {
            CAPTURE(idx, degree, g.degree(idx));
            CHECK(g.degree(idx) == degree);
        }
    }    
}

