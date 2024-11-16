#include "test_vertex.h"
#include <catch2/catch_test_macros.hpp>
#include <grafology/dense_graph_impl.h>

TEST_CASE("Hello World", "[hello]")
{
    REQUIRE(1 == 1);
}

void f() {
    g::DenseGraphImpl g(10, 5, true);
    g.set_edge(1, 2, 3);
    std::vector<g::edge_t> edges = {{1, 2, 3}, {2, 3, 4}};
    g.set_edges(edges.begin(), edges.end());
    g.set_edges(edges);
}