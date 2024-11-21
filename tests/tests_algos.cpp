#include <grafology/grafology.h>
#include <catch2/catch_template_test_macros.hpp>
#include <set>

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

TEMPLATE_TEST_CASE("Topological sort", "[graph-algos]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    std::vector<std::set<g::node_t>> expected {
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
    std::set<g::node_t> visited;
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
}
