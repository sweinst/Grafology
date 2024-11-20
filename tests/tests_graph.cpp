#include "test_vertex.h"
#include <grafology/graph.h>
#include <catch2/catch_template_test_macros.hpp>
#include <unordered_set>

namespace g = grafology;

TEMPLATE_TEST_CASE("Graphs", "[graphs]", 
    g::DirectedDenseGraph<TestVertex> , g::DirectedSparseGraph<TestVertex>,
    g::UndirectedDenseGraph<TestVertex> , g::UndirectedSparseGraph<TestVertex>) {

    using Edge = TestType::Edge;

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

    TestType graph(max_vertices);

    //initialization
    graph.add_vertices(init);
    graph.set_edges(edges_init);

    std::vector<unsigned> degrees = (graph.is_directed()) ? 
        std::vector<unsigned> { 
        2, 0, 3, 1, 0, 
        1 , 0} :  
        std::vector<unsigned>{ 
        2, 3, 4, 2, 1,
        2 };

    for (const auto [idx, degree]: std::views::enumerate(degrees)) {
        TestVertex v {static_cast<int>(idx), ""};
        CAPTURE(idx, degree, graph.degree(v));
        CHECK(graph.degree(v) == degree);
    }

    // modifications
    std::vector<unsigned> extra_degrees = graph.is_directed() ? 
        std::vector<unsigned> { 
        2, 0, 3, 2, 0, 
        2, 1, 0, 3, 0, 
        1 } :
        std::vector<unsigned>{ 
        2, 3, 4, 4, 1, 
        3, 1, 2, 4, 1, 
        2 };

    std::vector<unsigned> extra_in_degrees = graph.is_directed() ?
        std::vector<unsigned>{ 
            0, 3, 1, 2, 1, 
            1, 0, 2, 1, 1, 
            2 } :
        extra_degrees;

    graph.add_vertices(extra_init);
    graph.set_edges(extra_edges_init);
    CAPTURE(graph.size(), n_vertices + n_extra_vertices);
    REQUIRE(graph.size() == n_vertices + n_extra_vertices);

    for (const auto [idx, degree]: std::views::enumerate(extra_degrees)) {
        TestVertex v {static_cast<int>(idx), ""};
        CAPTURE(idx, degree, graph.degree(v));
        CHECK(graph.degree(v) == degree);
    }

    for (const auto [idx, in_degree]: std::views::enumerate(extra_in_degrees)) {
        TestVertex v {static_cast<int>(idx), ""};
        CAPTURE(idx, in_degree, graph.in_degree(v));
        CHECK(graph.in_degree(v) == in_degree);
    }

    // neighbors
    std::unordered_map<TestVertex, std::unordered_set<TestVertex>> neighbors;
    std::unordered_map<TestVertex, std::unordered_set<TestVertex>> in_neighbors;
    auto edges = {edges_init, extra_edges_init};
    for(const auto& edge: std::views::join(edges))
    {
        if (edge.start != edge.end) {
            neighbors[edge.start].insert(edge.end);
            in_neighbors[edge.end].insert(edge.start);
            if (!graph.is_directed()) {
                neighbors[edge.end].insert(edge.start);
                in_neighbors[edge.start].insert(edge.end);
            }
        }
    }
    for (const auto& v: graph.get_vertices())
    {
        {
            unsigned n_neighbors = 0;
            for (const auto& neighbor: graph.get_neighbors(v)) {
                CAPTURE(neighbor.start, neighbor.end, neighbor.weight);
                CHECK(neighbors[v].contains(neighbor.end));
                CHECK(neighbor.start._id + neighbor.end._id == neighbor.weight);
                ++n_neighbors;
            }
            CAPTURE(neighbors[v].size(), n_neighbors);
            CHECK(neighbors[v].size() == n_neighbors);
        }
        {
            unsigned n_neighbors = 0;
            for (const auto& neighbor: graph.get_raw_neighbors(v)) {
                CAPTURE(neighbor._id, v._id);
                CHECK(neighbors[v].contains(neighbor));
                ++n_neighbors;
            }
            CAPTURE(neighbors[v].size(), n_neighbors);
            CHECK(neighbors[v].size() == n_neighbors);
        }
        {
            unsigned n_in_neighbors = 0;
            for (const auto& in_neighbor: graph.get_in_neighbors(v)) {
                CAPTURE(in_neighbor.start._id, in_neighbor.end._id, in_neighbor.weight);
                CHECK(in_neighbors[v].contains(in_neighbor.start));
                CHECK(in_neighbor.start._id + in_neighbor.end._id == in_neighbor.weight);
                ++n_in_neighbors;
            }
            CAPTURE(in_neighbors[v].size(), n_in_neighbors);
            CHECK(in_neighbors[v].size() == n_in_neighbors);
        }
        {
            unsigned n_in_neighbors = 0;
            for (const auto& in_neighbor: graph.get_raw_in_neighbors(v)) {
                CAPTURE(in_neighbor._id, v._id);
                CHECK(in_neighbors[v].contains(in_neighbor));
                ++n_in_neighbors;
            }
            CAPTURE(in_neighbors[v].size(), n_in_neighbors);
            CHECK(in_neighbors[v].size() == n_in_neighbors);
        }
    }
}
