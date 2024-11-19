#include "test_vertex.h"
#include <grafology/graph.h>
#include <catch2/catch_template_test_macros.hpp>
#include <unordered_set>

namespace g = grafology;

namespace {
    using Edge = g::EdgeDefinition<TestVertex>;

    constexpr unsigned max_vertices = 11;
    constexpr unsigned n_vertices = 7;

    std::vector<TestVertex> init = {
      {0, "zero"},
      {1, "one"},
      {2, "two"},
      {3, "three"},
      {4, "four"},
      {5, "five"},
      {6, "six"},
    };

    // the Vertex key is in fact _id
    std::vector<Edge> edges_init = {
        {{0, ""}, {1, ""}, 1},
        {{0, ""}, {2, ""}, 2},
        {{2, ""}, {3, ""}, 5},
        {{2, ""}, {4, ""}, 6},
        {{2, ""}, {5, ""}, 7},
        {{3, ""}, {1, ""}, 4},
        {{5, ""}, {1, ""}, 6},
    };

    constexpr unsigned n_extra_vertices = 4;
    std::vector<TestVertex> extra_init = {
      {7, "seven"},
      {8, "eight"},
      {9, "nine"},
      {10, "ten"},
    };

    const std::vector<Edge> extra_edges_init {
        {{3, ""}, {10, ""}, 13},
        {{5, ""}, {8, ""}, 13},
        {{6, ""}, {7, ""}, 13},
        {{8, ""}, {3, ""}, 11},
        {{8, ""}, {7, ""}, 15},
        {{8, ""}, {9, ""}, 17},
        {{10, ""}, {10, ""}, 20},
    };
}

TEMPLATE_TEST_CASE("Directed graphs", "[graph]", g::DirectedDenseGraph<TestVertex> , g::DirectedSparseGraph<TestVertex>) {
    TestType graph(max_vertices);

    //initialization
    graph.add_vertices(init);
    graph.set_edges(edges_init);

    std::vector<unsigned> degrees { 
        2, 0, 3, 1, 0, 
        1 , 0};

    for (const auto [idx, degree]: std::views::enumerate(degrees))
    {
        TestVertex v {static_cast<int>(idx), ""};
        CAPTURE(idx, degree, graph.degree(v));
        CHECK(graph.degree(v) == degree);
    }

    // modifications
    std::vector<unsigned> extra_degrees { 
        2, 0, 3, 2, 0, 
        2, 1, 0, 3, 0, 
        1 };

    graph.add_vertices(extra_init);
    graph.set_edges(extra_edges_init);
    CAPTURE(graph.size(), n_vertices + n_extra_vertices);
    REQUIRE(graph.size() == n_vertices + n_extra_vertices);

    for (const auto [idx, degree]: std::views::enumerate(extra_degrees))
    {
        TestVertex v {static_cast<int>(idx), ""};
        CAPTURE(idx, degree, graph.degree(v));
        CHECK(graph.degree(v) == degree);
    }

    // neighbors
    std::unordered_map<TestVertex, std::unordered_set<TestVertex>> neighbors;
    auto edges = {edges_init, extra_edges_init};
    for(const auto& edge: std::views::join(edges))
    {
        if (edge.start != edge.end)
        {
            neighbors[edge.start].insert(edge.end);
        }
    }
    for (const auto& v: graph.get_vertices())
    {
        unsigned n_neighbors = 0;
        for (const auto& neighbor: graph.get_neighbors(v))
        {
            CAPTURE(neighbor.start, neighbor.end, neighbor.weight);
            CHECK(neighbors[v].contains(neighbor.end));
            CHECK(neighbor.start._id + neighbor.end._id == neighbor.weight);
            ++n_neighbors;
        }
        CAPTURE(neighbors[v].size(), n_neighbors);
        CHECK(neighbors[v].size() == n_neighbors);
    }
}

TEMPLATE_TEST_CASE("Undirected graphs", "[graph]", g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {
    TestType graph(max_vertices);

    //initialization
    graph.add_vertices(init);
    graph.set_edges(edges_init);

    std::vector<unsigned> degrees { 
        2, 3, 4, 2, 1,
        2 };

    for (const auto [idx, degree]: std::views::enumerate(degrees))
    {
        TestVertex v {static_cast<int>(idx), ""};
        CAPTURE(idx, degree, graph.degree(v));
        CHECK(graph.degree(v) == degree);
    }

    // modifications
    std::vector<unsigned> extra_degrees { 
        2, 3, 4, 4, 1, 
        3, 1, 2, 4, 1, 
        2 };

    graph.add_vertices(extra_init);
    graph.set_edges(extra_edges_init);
    CAPTURE(graph.size(), n_vertices + n_extra_vertices);
    REQUIRE(graph.size() == n_vertices + n_extra_vertices);

    for (const auto [idx, degree]: std::views::enumerate(extra_degrees))
    {
        TestVertex v {static_cast<int>(idx), ""};
        CAPTURE(idx, degree, graph.degree(v));
        CHECK(graph.degree(v) == degree);
    }

    // neighbors
    std::unordered_map<TestVertex, std::unordered_set<TestVertex>> neighbors;
    auto edges = {edges_init, extra_edges_init};
    for(const auto& edge: std::views::join(edges))
    {
        if (edge.start != edge.end)
        {
            neighbors[edge.start].insert(edge.end);
            neighbors[edge.end].insert(edge.start);
        }
    }
    for (const auto& v: graph.get_vertices())
    {
        unsigned n_neighbors = 0;
        for (const auto& neighbor: graph.get_neighbors(v))
        {
            CAPTURE(neighbor.start._id, neighbor.end._id, neighbor.weight);
            CHECK(neighbors[v].contains(neighbor.end));
            CHECK(neighbor.start._id + neighbor.end._id == neighbor.weight);
            ++n_neighbors;
        }
        CAPTURE(neighbors[v].size(), n_neighbors);
        CHECK(neighbors[v].size() == n_neighbors);
    }
}
