#include <grafology/algorithms/topological_sort.h>
#include <grafology/algorithms/depth_first_search.h>
#include <grafology/algorithms/breath_first_search.h>
#include <grafology/algorithms/transitive_closure.h>
#include <grafology/algorithms/minimum_spanning_tree.h>
#include <grafology/algorithms/all_shortest_paths.h>
#include <grafology/algorithms/shortest_path.h>
#include <catch2/catch_template_test_macros.hpp>
#include <set>
#include <print>
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

TEMPLATE_TEST_CASE("Impl - Transitive closure directed", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    const std::vector<g::edge_t> directed_extra_edges {
        {0, 3}, {0, 4}, {0, 5}, {0, 7}, {0, 8}, {0, 9}, {0, 10},
        {2, 1}, {2, 8}, {2, 7}, {2, 9}, {2, 10},
        {5, 1}, {5, 3}, {5, 7}, {5, 9}, {5, 10},
        {8, 1}, {8, 10},
    };

    TestType g(max_vertices, n_vertices, true);
    g.set_edges(edges_init);
    g::transitive_closure(g);

    TestType expected(max_vertices, n_vertices, true);
    expected.set_edges(edges_init);
    expected.set_edges(directed_extra_edges);

    for (g::vertex_t i = 0; i < n_vertices; ++i) {
        for (g::vertex_t j = 0; j < n_vertices; ++j) {
            CAPTURE(i, j);
            CHECK(g.has_edge(i, j) == expected.has_edge(i, j));
        }
    }
}

TEMPLATE_TEST_CASE("Impl - Transitive closure undirected", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    TestType g(max_vertices, n_vertices, false);
    g.set_edges(edges_init);
    g::transitive_closure(g);

    // all vertices should now be directly connected
    for (g::vertex_t i = 0; i < n_vertices; ++i) {
        for (g::vertex_t j = 0; j < n_vertices; ++j) {
            CAPTURE(i, j);
            CHECK(g.has_edge(i, j));
        }
    }
}

TEMPLATE_TEST_CASE("Impl - MST", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    std::vector<g::edge_t> edges {
        {0, 1 , 5},
        {0, 2 , 3},
        {3, 1 , 2},
        {4, 1 , 6},
        {5, 4 , 2},
        {5, 3 , 1},
        {1, 2 , 5},
        {4, 6 , 5},
        {6, 7 , 5},
        {7, 8 , 5},
        {10, 11 , 5},
        {10, 12 , 2},
        {12, 11 , 2},
        {11, 13 , 5},
        {14, 13 , 5},
    };

    std::vector<g::edge_t> expected_edges {
        {0, 1 , 5},
        {0, 2 , 3},
        {3, 1 , 2},
        {5, 4 , 2},
        {5, 3 , 1},
        {4, 6 , 5},
        {6, 7 , 5},
        {7, 8 , 5},
        {10, 12 , 2},
        {12, 11 , 2},
        {11, 13 , 5},
        {14, 13 , 5},
    };


    TestType g(15, 15, false);
    g.set_edges(edges);

    auto mst = g::minimum_spanning_tree(g);

    TestType expected(15, 15, false);
    expected.set_edges(expected_edges);
    REQUIRE(mst == expected);
}

TEMPLATE_TEST_CASE("Impl - Dijkstra", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    int n_vertices=12;
    std::vector<g::edge_t> edges = {
        {0, 1, 4}, {0, 7, 8},
        {1, 2, 8}, {1, 7, 11},
        {2, 3, 7}, {2, 5, 4}, {2, 8, 2},
        {3, 4, 9}, {3, 5, 14},
        {4, 5, 10},
        {5, 6, 2},
        {6, 7, 1}, {6, 8, 6},
        {7, 8, 7},
        {9, 10, 4},
        {9, 11, 2},
        {11, 10, 1},
        };

    std::vector<g::weight_t> expected_distances = {0, 4, 12, 19, 21, 11, 9, 8, 14, g::D_INFINITY, g::D_INFINITY, g::D_INFINITY};
    std::vector<g::vertex_t> expected_predecessors = { g::NO_PREDECESSOR, 0, 1, 2, 5, 6, 7, 0, 2, g::NO_PREDECESSOR, g::NO_PREDECESSOR, g::NO_PREDECESSOR};
    std::vector<g::step_t> expected_path_to_8 = {{0, 0}, {1, 4}, {2, 12}, {8, 14}};
    std::vector<g::step_t> expected_path_to_5 = {{0, 0}, {7, 8}, {6, 9}, {5, 11}};
    const std::set<g::vertex_t> unreachable {9, 10, 11};

    TestType g(n_vertices, n_vertices, false);
    g.set_edges(edges);
    auto paths = g::all_shortest_paths(g, 0);

    CHECK(paths._distances == expected_distances);
    CHECK(paths._predecessors == expected_predecessors);
    auto path_to_8 = paths.get_path(8);
    CHECK(path_to_8 == expected_path_to_8);
    auto path_to_5 = paths.get_path(5);
    CHECK(path_to_5 == expected_path_to_5);
    for (g::vertex_t i = 0; i < n_vertices; ++i)
    {
        CAPTURE(i);
        CHECK(paths.is_reachable(i) == !unreachable.contains(i));
    }
    
}

TEMPLATE_TEST_CASE("Impl - A*", "[impl-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    int n_vertices=12;
    std::vector<g::edge_t> edges = {
        {0, 1, 4}, {0, 7, 8},
        {1, 2, 8}, {1, 7, 11},
        {2, 3, 7}, {2, 5, 4}, {2, 8, 2},
        {3, 4, 9}, {3, 5, 14},
        {4, 5, 10},
        {5, 6, 2},
        {6, 7, 1}, {6, 8, 6},
        {7, 8, 7},
        {9, 10, 4},
        {9, 11, 2},
        {11, 10, 1},
        };


    TestType g(n_vertices, n_vertices, false);
    g.set_edges(edges);

    std::vector<std::tuple<
        // start
        g::vertex_t, 
        // end
        g::vertex_t,
        // path = vector<vertex, distance from start>
        std::vector<g::step_t>
        >> expected = {
        {0, 8, {{0, 0}, {1, 4}, {2, 12}, {8, 14}}},
        {0, 5, {{0, 0}, {7, 8}, {6, 9}, {5, 11}}},
        {4, 2, {{4, 0}, {5, 10}, {2, 14}}},
        {0, 11, {}},
    };
    
    for (auto [start, end, expected_path]: expected) {
        CAPTURE(start, end);
        // build our cost function from the real distances
        auto paths_to_end = g::all_shortest_paths(g, end);
        auto cost_function = [&paths_to_end](g::vertex_t i , g::vertex_t /* j */) {
            return paths_to_end._distances[i];
        };

        auto path = g::shortest_path(g, start, end, cost_function);
        CHECK(path.get_path() == expected_path);
    }
}
