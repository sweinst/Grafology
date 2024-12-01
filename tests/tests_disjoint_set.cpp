#include <grafology/disjoint_set.h>
#include <catch2/catch_test_macros.hpp>

namespace g = grafology;

TEST_CASE("Test DisjointSet", "[disjointset]")
{
    constexpr size_t n_vertices = 15;
    const std::vector <g::edge_t> edges = {
        {0, 1}, {0, 2}, {1, 2}, {1, 4}, {1, 3},
        {4, 5}, {3,5}, {4, 6}, {6, 7}, {7, 8},
        {14, 13}, {13, 11}, {11, 12}, {11, 10,}, {12, 10},
    };

    g::DisjointSet ds(n_vertices);
    for (const auto& edge : edges) {
        ds.merge(edge.start, edge.end);
    }

    int n_disjoint_sets = 0;
    for (const auto& root : ds.roots()) {
        ++n_disjoint_sets;
    }
    REQUIRE(n_disjoint_sets == 3);

    CHECK(ds.find(0) == ds.find(5));
    CHECK(ds.find(2) == ds.find(6));
    CHECK(ds.find(9) == 9);
    CHECK(ds.find(10) == ds.find(13));
    CHECK(ds.find(11) == ds.find(12));
}
