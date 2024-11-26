#include <grafology/algorithms/topological_sort.h>
#include <grafology/algorithms/depth_first_search.h>
#include <grafology/algorithms/breath_first_search.h>
#include <catch2/catch_template_test_macros.hpp>
#include <set>
#include "test_vertex.h"

namespace g = grafology;

namespace {
    constexpr unsigned max_vertices = 11;
    constexpr unsigned n_vertices = 11;

    const std::vector<g::edge_t> edges_init {
        {0, 1, 1},
        {0, 2, 2},
        {2, 3, 5},
        {2, 4, 6},
        {2, 5, 7},
        {3, 1, 4},
        {5, 1, 6},
        {3, 10, 13},
        {5, 8, 13},
        {6, 7, 13},
        {8, 3, 11},
        {8, 7, 15},
        {8, 9, 17},
    };
   
}

TEMPLATE_TEST_CASE("Impl - Topological sort", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    const std::vector<std::set<g::vertex_t>> expected {
        {0, 6},
        {2},
        {4, 5},
        {8},
        {3, 7, 9},
        {1, 10},  
    };

    TestType g(max_vertices, n_vertices, true);
    g.set_edges(edges_init);

    unsigned current_group = 0;
    std::set<g::vertex_t> visited;
    for (auto [group, vertex]: g::topological_sort(g)) {
        if (group != current_group) {
            CHECK(expected[current_group] == visited);
            current_group = group;
            visited.clear();
        }
        CAPTURE(group, vertex);
        REQUIRE(group < expected.size());
        CHECK(expected[group].contains(vertex));
        visited.insert(vertex);
    }
    CHECK(expected[current_group] == visited);

    // add a cycle
    const std::vector<g::edge_t> edges_cycle {
        {1, 7, 8},
        {7, 5, 12},
    };
    g.set_edges(edges_cycle);
    
    REQUIRE_THROWS(std::ranges::for_each(g::topological_sort(g), [](const auto& group_vertex) {
        CAPTURE(group_vertex);
    }));
    
}

TEMPLATE_TEST_CASE("Impl - DFS", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    const std::vector<g::vertex_t> expected_directed {
        0, 2, 5, 8, 9, 7, 3, 10, 1, 4 
    };
    const std::vector<g::vertex_t> expected_undirected {
        0, 2, 5, 8, 9, 7, 6, 3, 10, 1, 4 
    };

    for(auto is_directed : {false, true}) {
        CAPTURE(is_directed);

        TestType g(max_vertices, n_vertices, is_directed);
        g.set_edges(edges_init);

        unsigned current_group = 0;
        std::vector<g::vertex_t> visited;
        for (auto vertex: g::depth_first_search(g, 0)) {
            CAPTURE(vertex);
            CHECK(std::ranges::find(visited, vertex) == visited.end());
            visited.push_back(vertex);
        }
        CHECK(visited == (is_directed ? expected_directed : expected_undirected));
    }
}

TEMPLATE_TEST_CASE("Impl - BFS", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    const std::vector<g::vertex_t> expected_directed {
        0, 1, 2, 3, 4, 5, 10, 8, 7, 9 
    };
    const std::vector<g::vertex_t> expected_undirected {
        0, 1, 2, 3, 5, 4, 8, 10, 7, 9, 6 
    };

    for(auto is_directed : {false, true}) {
        CAPTURE(is_directed);

        TestType g(max_vertices, n_vertices, is_directed);
        g.set_edges(edges_init);

        unsigned current_group = 0;
        std::vector<g::vertex_t> visited;
        for (auto vertex: g::breath_first_search(g, 0)) {
            CAPTURE(vertex);
            CHECK(std::ranges::find(visited, vertex) == visited.end());
            visited.push_back(vertex);
        }
        CHECK(visited == (is_directed ? expected_directed : expected_undirected));
    }
}
