#include <grafology/algorithms/topological_sort.h>
#include <grafology/algorithms/depth_first_search.h>
#include <grafology/algorithms/breath_first_search.h>
#include <catch2/catch_template_test_macros.hpp>
#include <unordered_set>
#include "test_vertex.h"
using namespace std::string_literals;

namespace g = grafology;
using TestEdge = g::EdgeDefinition<TestVertex>;

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
        {{0, ""}, {1, ""}, 1},
        {{0, ""}, {2, ""}, 2},
        {{2, ""}, {3, ""}, 5},
        {{2, ""}, {4, ""}, 6},
        {{2, ""}, {5, ""}, 7},
        {{3, ""}, {1, ""}, 4},
        {{5, ""}, {1, ""}, 6},
        {{3, ""}, {10, ""}, 13},
        {{5, ""}, {8, ""}, 13},
        {{6, ""}, {7, ""}, 13},
        {{8, ""}, {3, ""}, 11},
        {{8, ""}, {7, ""}, 15},
        {{8, ""}, {9, ""}, 17},
        {{10, ""}, {10, ""}, 20},
    };
}


TEMPLATE_TEST_CASE("Graphs - Topological sort", "[graphs-algos]", 
    g::DirectedDenseGraph<TestVertex> , g::DirectedSparseGraph<TestVertex>) {

    const std::vector<std::unordered_set<TestVertex>> expected {
        {{0, ""}, {6, ""}},
        {{2, ""}},
        {{4, ""}, {5, ""}},
        {{8, ""}},
        {{3, ""}, {7, ""}, {9, ""}},
        {{1, ""}, {10, ""}},  
    };

    //initialization
    TestType graph(max_vertices);
    graph.add_vertices(vertices_init);
    graph.set_edges(edges_init);

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
        {{1, ""}, {7, ""}, 8},
        {{7, ""}, {5, ""}, 12},
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
        {0, ""s}, {2, ""s}, {5, ""s}, {8, ""s}, 
        {9, ""s}, {7, ""s}, {3, ""s}, {10, ""s}, 
        {1, ""s}, {4, ""s},
    };
    const std::vector<TestVertex> undirected_expected {
        {0, ""s}, {2, ""s}, {5, ""s}, {8, ""s}, 
        {9, ""s}, {7, ""s}, {6, ""s}, {3, ""s}, 
        {10, ""s}, {1, ""s}, {4, ""s}
    };

    TestType graph(max_vertices);
    graph.set_edges(edges_init, true);

    std::vector<TestVertex> visited;
    for (const auto& vertex: g::depth_first_search(graph, TestVertex{0, ""})) {
        CAPTURE(vertex);
        visited.push_back(vertex);
    }
    CHECK(visited == (graph.is_directed() ? directed_expected : undirected_expected));
 }

TEMPLATE_TEST_CASE("Graphs - BFS", "[graphs-algos]", 
    g::DirectedDenseGraph<TestVertex> , g::DirectedSparseGraph<TestVertex>,
    g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {

    const std::vector<TestVertex> directed_expected {
        {0, ""s}, {1, ""s}, {2, ""s}, {3, ""s}, 
        {4, ""s}, {5, ""s}, {10, ""s}, {8, ""s}, 
        {7, ""s}, {9, ""s},
    };
    const std::vector<TestVertex> undirected_expected {
        {0, ""s}, {1, ""s}, {2, ""s}, {3, ""s}, 
        {5, ""s}, {4, ""s}, {10, ""s}, {8, ""s}, 
        {7, ""s}, {9, ""s}, {6, ""s},
    };

    TestType graph(max_vertices);
    graph.set_edges(edges_init, true);

    std::vector<TestVertex> visited;
    for (const auto& vertex: g::breath_first_search(graph, TestVertex{0, ""})) {
        CAPTURE(vertex);
        visited.push_back(vertex);
    }
    CHECK(visited == (graph.is_directed() ? directed_expected : undirected_expected));
 }
