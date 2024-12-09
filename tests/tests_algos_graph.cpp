#include <grafology/algorithms/topological_sort.h>
#include <grafology/algorithms/depth_first_search.h>
#include <grafology/algorithms/breath_first_search.h>
#include <grafology/algorithms/minimum_spanning_tree.h>
#include <grafology/algorithms/all_shortest_paths.h>
#include <grafology/algorithms/shortest_path.h>
#include <catch2/catch_template_test_macros.hpp>
#include <unordered_set>
#include "test_vertex.h"
using namespace std::string_literals;

namespace g = grafology;
using TestEdge = g::EdgeDefinition<TestVertex>;

namespace {
    std::vector<TestVertex> generate_test_vertices_list(int n_vertices) {
        std::vector<TestVertex> vertices_init;
        for(int i = 0; i < n_vertices; ++i) {
            TestVertex v {i, std::to_string(i)};
            vertices_init.push_back(v);
        }
        return vertices_init;
    }
}   // namespace

namespace {
    constexpr unsigned max_vertices = 11;
    constexpr unsigned n_vertices = 11;

    const std::vector<TestVertex> vertices_init = {
      {0, "zero"},
      {1, "one"},
      {2, "two"},
      {3, "three"},
      {4, "four"},
      {5, "five"},
      {6, "six"},
      {7, "seven"},
      {8, "eight"},
      {9, "nine"},
      {10, "ten"},
    };

    const std::vector<TestEdge> edges_init = {
        {{0}, {1}, 1},
        {{0}, {2}, 2},
        {{2}, {3}, 5},
        {{2}, {4}, 6},
        {{2}, {5}, 7},
        {{3}, {1}, 4},
        {{5}, {1}, 6},
        {{3}, {10}, 13},
        {{5}, {8}, 13},
        {{6}, {7}, 13},
        {{8}, {3}, 11},
        {{8}, {7}, 15},
        {{8}, {9}, 17},
        {{10}, {10}, 20},
    };
}


TEMPLATE_TEST_CASE("Graphs - Topological sort", "[graphs-algos]", 
    g::DirectedDenseGraph<TestVertex> , g::DirectedSparseGraph<TestVertex>) {

    const std::vector<std::unordered_set<TestVertex>> expected {
        {{0}, {6}},
        {{2}},
        {{4}, {5}},
        {{8}},
        {{3}, {7}, {9}},
        {{1}, {10}},  
    };

    //initialization
    TestType graph(max_vertices);
    graph.add_vertices(vertices_init);
    graph.set_edges(edges_init);

    // remove the only cycle
    graph.set_edge({10}, {10}, 0);

    unsigned current_group = 0;
    std::unordered_set<TestVertex> visited;
    for (const auto& [group, vertex]: g::topological_sort(graph)) {
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
    const std::vector<TestEdge> edges_cycle {
        {{1}, {7}, 8},
        {{7}, {5}, 12},
    };
    graph.set_edges(edges_cycle);
    
    REQUIRE_THROWS(std::ranges::for_each(g::topological_sort(graph), [](const auto& group_vertex) {
        CAPTURE(group_vertex);
    }));
    
}

TEMPLATE_TEST_CASE("Graphs - DFS", "[graphs-algos]", 
    g::DirectedDenseGraph<TestVertex> , g::DirectedSparseGraph<TestVertex>,
    g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {

    const std::vector<TestVertex> directed_expected {
        {0}, {2}, {5}, {8}, 
        {9}, {7}, {3}, {10}, 
        {1}, {4},
    };
    const std::vector<TestVertex> undirected_expected {
        {0}, {2}, {5}, {8}, 
        {9}, {7}, {6}, {3}, 
        {10}, {1}, {4}
    };

    TestType graph(max_vertices);
    graph.set_edges(edges_init, true);

    std::vector<TestVertex> visited;
    for (const auto& vertex: g::depth_first_search(graph, TestVertex{0})) {
        CAPTURE(vertex);
        visited.push_back(vertex);
    }
    CHECK(visited == (graph.is_directed() ? directed_expected : undirected_expected));
 }

TEMPLATE_TEST_CASE("Graphs - BFS", "[graphs-algos]", 
    g::DirectedDenseGraph<TestVertex> , g::DirectedSparseGraph<TestVertex>,
    g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {

    const std::vector<TestVertex> directed_expected {
        {0}, {1}, {2}, {3}, 
        {4}, {5}, {10}, {8}, 
        {7}, {9},
    };
    const std::vector<TestVertex> undirected_expected {
        {0}, {1}, {2}, {3}, 
        {5}, {4}, {10}, {8}, 
        {7}, {9}, {6},
    };

    TestType graph(max_vertices);
    graph.set_edges(edges_init, true);

    std::vector<TestVertex> visited;
    for (const auto& vertex: g::breath_first_search(graph, TestVertex{0})) {
        CAPTURE(vertex);
        visited.push_back(vertex);
    }
    CHECK(visited == (graph.is_directed() ? directed_expected : undirected_expected));
 }

TEMPLATE_TEST_CASE("Graphs - NST", "[graphs-algos]", 
    g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {
    unsigned n_vertices = 15;
    std::vector<TestVertex> vertices_init {{ generate_test_vertices_list(n_vertices) }};

    std::vector<TestEdge> edges_init {
        {{0}, {1}, 5},
        {{0}, {2}, 3},
        {{3}, {1}, 2},
        {{4}, {1}, 6},
        {{5}, {4}, 2},
        {{5}, {3}, 1},
        {{1}, {2}, 5},
        {{4}, {6}, 5},
        {{6}, {7}, 5},
        {{7}, {8}, 5},
        {{10}, {11}, 5},
        {{10}, {12}, 2},
        {{12}, {11}, 2},
        {{11}, {13}, 5},
        {{14}, {13}, 5},
    };

    std::vector<TestEdge> expected_edges {
         {{0}, {1}, 5},
        {{0}, {2}, 3},
        {{3}, {1}, 2},
        {{5}, {4}, 2},
        {{5}, {3}, 1},
        {{4}, {6}, 5},
        {{6}, {7}, 5},
        {{7}, {8}, 5},
        {{10}, {12}, 2},
        {{12}, {11}, 2},
        {{11}, {13}, 5},
        {{14}, {13}, 5},
   };

    TestType g(n_vertices);
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    auto mst = g::minimum_spanning_tree(g);

    TestType expected(n_vertices);
    expected.add_vertices(vertices_init);
    expected.set_edges(expected_edges);

    REQUIRE(mst.impl() == expected.impl());
    REQUIRE(mst == expected);
}

TEMPLATE_TEST_CASE("Graphs - Dijkstra", "[graphs-algos]", 
    g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {

    int n_vertices=12;

    std::vector<TestVertex> vertices_init {{ generate_test_vertices_list(n_vertices) }};
    std::vector<TestEdge> edges_init = {
        {{0}, {1}, 4}, {{0}, {7}, 8},
        {{1}, {2}, 8}, {{1}, {7}, 11},
        {{2}, {3}, 7}, {{2}, {5}, 4}, {{2}, {8}, 2},
        {{3}, {4}, 9}, {{3}, {5}, 14},
        {{4}, {5}, 10},
        {{5}, {6}, 2},
        {{6}, {7}, 1}, {{6}, {8}, 6},
        {{7}, {8}, 7},
        {{9}, {10}, 4},
        {{9}, {11}, 2},
        {{11}, {10}, 1},
        };

    std::vector<g::weight_t> expected_distances = {0, 4, 12, 19, 21, 11, 9, 8, 14, g::D_INFINITY, g::D_INFINITY, g::D_INFINITY};
    std::vector<TestVertex> expected_predecessors = { /* starting at node 1 */{0}, {1}, {2}, {5}, {6}, {7}, {0}, {2}, {-1}, {-1}, {-1}};
    std::vector<g::Step<TestVertex>> expected_path_to_8 = {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{8}, 14}};
    std::vector<g::Step<TestVertex>> expected_path_to_5 = {{{0}, 0}, {{7}, 8}, {{6}, 9}, {{5}, 11}};
    std::unordered_set<TestVertex> unreachable {{9}, {10}, {11}};

    TestType g(n_vertices);
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    auto paths = g::all_shortest_paths(g, {0});

    for(int i = 0; i < n_vertices; ++i) {
        CAPTURE(i);
        CHECK(paths.get_distance(vertices_init[i]) == expected_distances[i]);
        CHECK(paths.is_reachable(vertices_init[i]) == !unreachable.contains(vertices_init[i]));
        if (i != 0) {
            if (paths.is_reachable(vertices_init[i])) {
                CHECK(paths.get_predecessor(vertices_init[i]) == expected_predecessors[i-1]);
            }
        }
    }

    for(const auto& [idx, v]: std::views::enumerate(paths.get_path(vertices_init[8]))) {
        CAPTURE(idx, v, expected_path_to_8[idx]);
        CHECK(v == expected_path_to_8[idx]);
    }

    for(const auto& [idx, v]: std::views::enumerate(paths.get_path(vertices_init[5]))) {
        CAPTURE(idx, v, expected_path_to_8[idx]);
        CHECK(v == expected_path_to_5[idx]);
    }
}

TEMPLATE_TEST_CASE("Graphs - A*", "[graphs-algos]", 
    g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {

    int n_vertices=12;

    std::vector<TestVertex> vertices_init {{ generate_test_vertices_list(n_vertices) }};
    std::vector<TestEdge> edges_init = {
        {{0}, {1}, 4}, {{0}, {7}, 8},
        {{1}, {2}, 8}, {{1}, {7}, 11},
        {{2}, {3}, 7}, {{2}, {5}, 4}, {{2}, {8}, 2},
        {{3}, {4}, 9}, {{3}, {5}, 14},
        {{4}, {5}, 10},
        {{5}, {6}, 2},
        {{6}, {7}, 1}, {{6}, {8}, 6},
        {{7}, {8}, 7},
        {{9}, {10}, 4},
        {{9}, {11}, 2},
        {{11}, {10}, 1},
        };

    TestType g(n_vertices);
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    std::vector<std::tuple<
        // start
        TestVertex, 
        // end
        TestVertex,
        // path = vector<vertex, distance from start>
        std::vector<std::tuple<TestVertex, g::weight_t>>
        >> expected = {
        {{0}, {8}, {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{8}, 14}}},
        {{0}, {5}, {{{0}, 0}, {{7}, 8}, {{6},9}, {{5}, 11}}},
        {{4}, {2}, {{{4}, 0}, {{5}, 10}, {{2}, 14}}},
        {{0}, {11}, {}},
    };

    for (const auto& [start, end, expected_path]: expected) {
        CAPTURE(start, end);
        // build our cost function from the real distances
        auto paths_to_end = g::all_shortest_paths(g, end);
        auto cost_function = [&paths_to_end](const TestVertex& i , const TestVertex& /* j */) {
            return paths_to_end.get_distance(i);
        };

        auto path = g::shortest_path(g, start, end, cost_function);
        auto v_path = path | std::ranges::to<std::vector>();
        CHECK(v_path == expected_path);
    }
}
