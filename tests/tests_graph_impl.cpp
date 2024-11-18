#include "test_vertex.h"
#include <catch2/catch_template_test_macros.hpp>
#include <grafology/dense_graph_impl.h>
#include <grafology/sparse_graph_impl.h>
#include <iostream>

TEMPLATE_TEST_CASE("Graph implementations", "[graph-impl]", g::DenseGraphImpl , g::SparseGraphImpl)
{
    constexpr unsigned max_vertices = 11;
    constexpr unsigned n_vertices = 7;
    const std::vector<g::edge_t> edges_init {
        {0, 1, 1},
        {0, 2, 2},
        {2, 3, 5},
        {2, 4, 6},
        {2, 5, 7},
        {3, 1, 4},
        {5, 1, 6},
    };
    constexpr unsigned n_extra_vertices = 4;
    const std::vector<g::edge_t> extra_edges_init {
        {3, 10, 11},
        {5, 8, 13},
        {6, 7, 13},
        {8, 3, 11},
        {8, 7, 15},
        {8, 9, 17},
    };


    SECTION("Directed graphs")
    {   
        std::vector<unsigned> degrees { 
            2, 0, 3, 1, 0, 
            1 , 0};

        TestType g(max_vertices, n_vertices, true);
        g.set_edges(edges_init);
        for (const auto [idx, degree]: std::views::enumerate(degrees))
        {
            CAPTURE(idx, degree, g.degree(idx));
            CHECK(g.degree(idx) == degree);
        }

        std::vector<unsigned> extra_degrees { 
            2, 0, 3, 2, 0, 
            2, 1, 0, 3, 0, 
            0 };

        for (auto _ : g.add_vertices(n_extra_vertices)) {};
            CAPTURE(g.size(), n_vertices + n_extra_vertices);
            REQUIRE(g.size() == n_vertices + n_extra_vertices);
        for (const auto& edge: extra_edges_init)
        {
            g.set_edge(edge);
        }

        for (const auto [idx, degree]: std::views::enumerate(extra_degrees))
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
        std::vector<unsigned> extra_degrees { 
            2, 3, 4, 4, 1, 
            3, 1, 2, 4, 1, 
            1 };


        for (auto _ : g.add_vertices(n_extra_vertices)) {}
        CAPTURE(g.size(), n_vertices + n_extra_vertices);
        REQUIRE(g.size() == n_vertices + n_extra_vertices);
        for (const auto& edge: extra_edges_init)
        {
            g.set_edge(edge);
        }

        for (const auto [idx, degree]: std::views::enumerate(extra_degrees))
        {
            CAPTURE(idx, degree, g.degree(idx));
            CHECK(g.degree(idx) == degree);
        }
    }    
}

