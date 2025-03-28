#include <grafology/algorithms/all_shortest_paths.h>
#include <grafology/algorithms/articulation_points.h>
#include <grafology/algorithms/breath_first_search.h>
#include <grafology/algorithms/bridges.h>
#include <grafology/algorithms/depth_first_search.h>
#include <grafology/algorithms/maximum_flow.h>
#include <grafology/algorithms/minimum_spanning_tree.h>
#include <grafology/algorithms/shortest_path.h>
#include <grafology/algorithms/strongly_connected_components.h>
#include <grafology/algorithms/topological_sort.h>
#include <catch2/catch_template_test_macros.hpp>
#include <unordered_set>
#include "test_vertex.h"
using namespace std::string_literals;
#include <grafology/graph_traits.h>

namespace g = grafology;
using weight_t = int;
using TestEdge = g::EdgeDefinition<TestVertex, weight_t>;
using DirectedDenseGraph = g::DirectedDenseGraph<TestVertex, weight_t>;
using DirectedSparseGraph = g::DirectedSparseGraph<TestVertex, weight_t>;
using UndirectedDenseGraph = g::UndirectedDenseGraph<TestVertex, weight_t>;
using UndirectedSparseGraph = g::UndirectedSparseGraph<TestVertex, weight_t>;
using Step = g::Step<TestVertex, weight_t>;
static constexpr auto D_INFINITY = g::edge_t<weight_t>::D_INFINITY;

namespace {
    std::vector<TestVertex> generate_test_vertices_list(int n_vertices) {
        std::vector<TestVertex> vertices_init;
        for (unsigned i = 0; i < n_vertices; ++i) {
            TestVertex v{i, std::to_string(i)};
            vertices_init.push_back(v);
        }
        return vertices_init;
    }
}  // namespace

namespace {
    constexpr unsigned max_vertices = 11;
    constexpr unsigned n_vertices = 11;

    const std::vector<TestVertex> vertices_init = {
        {0, "zero"}, {1, "one"},   {2, "two"},   {3, "three"}, {4, "four"}, {5, "five"},
        {6, "six"},  {7, "seven"}, {8, "eight"}, {9, "nine"},  {10, "ten"},
    };

    const std::vector<TestEdge> edges_init = {
        {{0}, {1}, 1},  {{0}, {2}, 2},  {{2}, {3}, 5},   {{2}, {4}, 6},    {{2}, {5}, 7},
        {{3}, {1}, 4},  {{5}, {1}, 6},  {{3}, {10}, 13}, {{5}, {8}, 13},   {{6}, {7}, 13},
        {{8}, {3}, 11}, {{8}, {7}, 15}, {{8}, {9}, 17},  {{10}, {10}, 20},
    };
}  // namespace

TEMPLATE_TEST_CASE(
    "Graphs - Topological sort",
    "[graphs-algos]",
    DirectedDenseGraph,
    DirectedSparseGraph
) {
    const std::vector<std::unordered_set<TestVertex>> expected{
        {{0}, {6}}, {{2}}, {{4}, {5}}, {{8}}, {{3}, {7}, {9}}, {{1}, {10}},
    };

    // initialization
    TestType graph(max_vertices);
    graph.add_vertices(vertices_init);
    graph.set_edges(edges_init);

    // remove the only cycle
    graph.set_edge({10}, {10}, 0);

    unsigned current_group = 0;
    std::unordered_set<TestVertex> visited;
    for (const auto& [group, vertex] : g::topological_sort(graph)) {
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
    const std::vector<TestEdge> edges_cycle{
        {{1}, {7}, 8},
        {{7}, {5}, 12},
    };
    graph.set_edges(edges_cycle);

    REQUIRE_THROWS(std::ranges::for_each(g::topological_sort(graph), [](const auto& group_vertex) {
        CAPTURE(group_vertex);
    }));
}

TEMPLATE_TEST_CASE(
    "Graphs - DFS",
    "[graphs-algos]",
    DirectedDenseGraph,
    DirectedSparseGraph,
    UndirectedDenseGraph,
    UndirectedSparseGraph
) {
    const std::vector<TestVertex> directed_expected{
        {0}, {2}, {5}, {8}, {9}, {7}, {3}, {10}, {1}, {4},
    };
    const std::vector<TestVertex> undirected_expected{{0}, {2}, {5},  {8}, {9}, {7},
                                                      {6}, {3}, {10}, {1}, {4}};

    TestType graph(max_vertices);
    graph.set_edges(edges_init, true);

    std::vector<TestVertex> visited;
    for (const auto& vertex : g::depth_first_search(graph, TestVertex{0})) {
        CAPTURE(vertex);
        visited.push_back(vertex);
    }
    CHECK(visited == (graph.is_directed() ? directed_expected : undirected_expected));
}

TEMPLATE_TEST_CASE(
    "Graphs - BFS",
    "[graphs-algos]",
    DirectedDenseGraph,
    DirectedSparseGraph,
    UndirectedDenseGraph,
    UndirectedSparseGraph
) {
    const std::vector<TestVertex> directed_expected{
        {0}, {1}, {2}, {3}, {4}, {5}, {10}, {8}, {7}, {9},
    };
    const std::vector<TestVertex> undirected_expected{
        {0}, {1}, {2}, {3}, {5}, {4}, {10}, {8}, {7}, {9}, {6},
    };

    TestType graph(max_vertices);
    graph.set_edges(edges_init, true);

    std::vector<TestVertex> visited;
    for (const auto& vertex : g::breath_first_search(graph, TestVertex{0})) {
        CAPTURE(vertex);
        visited.push_back(vertex);
    }
    CHECK(visited == (graph.is_directed() ? directed_expected : undirected_expected));
}

TEMPLATE_TEST_CASE("Graphs - NST", "[graphs-algos]", UndirectedDenseGraph, UndirectedSparseGraph) {
    unsigned n_vertices = 15;
    std::vector<TestVertex> vertices_init{{generate_test_vertices_list(n_vertices)}};

    std::vector<TestEdge> edges_init{
        {{0}, {1}, 5},   {{0}, {2}, 3},   {{3}, {1}, 2},   {{4}, {1}, 6},   {{5}, {4}, 2},
        {{5}, {3}, 1},   {{1}, {2}, 5},   {{4}, {6}, 5},   {{6}, {7}, 5},   {{7}, {8}, 5},
        {{10}, {11}, 5}, {{10}, {12}, 2}, {{12}, {11}, 2}, {{11}, {13}, 5}, {{14}, {13}, 5},
    };

    std::vector<TestEdge> expected_edges{
        {{0}, {1}, 5},   {{0}, {2}, 3},   {{3}, {1}, 2},   {{5}, {4}, 2},
        {{5}, {3}, 1},   {{4}, {6}, 5},   {{6}, {7}, 5},   {{7}, {8}, 5},
        {{10}, {12}, 2}, {{12}, {11}, 2}, {{11}, {13}, 5}, {{14}, {13}, 5},
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

TEMPLATE_TEST_CASE(
    "Graphs - Dijkstra",
    "[graphs-algos]",
    UndirectedDenseGraph,
    UndirectedSparseGraph,
    DirectedDenseGraph,
    DirectedSparseGraph
) {
    int n_vertices = 12;

    std::vector<TestVertex> vertices_init{{generate_test_vertices_list(n_vertices)}};
    std::vector<TestEdge> edges_init = {
        {{0}, {1}, 4},  {{0}, {7}, 8},   {{1}, {2}, 8}, {{1}, {7}, 11}, {{2}, {3}, 7},
        {{2}, {5}, 4},  {{2}, {8}, 2},   {{3}, {4}, 9}, {{3}, {5}, 14}, {{4}, {5}, 10},
        {{5}, {6}, 2},  {{6}, {7}, 1},   {{6}, {8}, 6}, {{7}, {8}, 7},  {{9}, {10}, 4},
        {{9}, {11}, 2}, {{11}, {10}, 1},
    };

    std::vector<weight_t> expected_distances[2]{
        {0, 4, 12, 19, 21, 11, 9, 8, 14, D_INFINITY, D_INFINITY, D_INFINITY},
        {0, 4, 12, 19, 28, 16, 18, 8, 14, D_INFINITY, D_INFINITY, D_INFINITY},
    };
    std::vector<TestVertex> expected_predecessors[2] = {
        {{g::NO_PREDECESSOR},
         {0},
         {1},
         {2},
         {5},
         {6},
         {7},
         {0},
         {2},
         {g::NO_PREDECESSOR},
         {g::NO_PREDECESSOR},
         {g::NO_PREDECESSOR}},
        {{g::NO_PREDECESSOR},
         {0},
         {1},
         {2},
         {3},
         {2},
         {5},
         {0},
         {2},
         {g::NO_PREDECESSOR},
         {g::NO_PREDECESSOR},
         {g::NO_PREDECESSOR}},
    };
    std::vector<Step> expected_path_to_8[2] = {
        {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{8}, 14}},
        {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{8}, 14}},
    };
    std::vector<Step> expected_path_to_5[2] = {
        {{{0}, 0}, {{7}, 8}, {{6}, 9}, {{5}, 11}},
        {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{5}, 16}},
    };

    std::unordered_set<TestVertex> unreachable{{9}, {10}, {11}};

    TestType g(n_vertices);
    auto directed = g.is_directed();
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    for (auto use_dijkstra_algo : {true, false}) {
        CAPTURE(use_dijkstra_algo);
        if (!use_dijkstra_algo and !directed) {
            // Bellman-Ford only works with directed graphs
            continue;
        }
        auto paths = g::all_shortest_paths(g, {0});
        for (int i = 0; i < n_vertices; ++i) {
            CAPTURE(i);
            CHECK(paths.get_distance(vertices_init[i]) == expected_distances[directed][i]);
            CHECK(paths.is_reachable(vertices_init[i]) == !unreachable.contains(vertices_init[i]));
            if (i != 0) {
                if (paths.is_reachable(vertices_init[i])) {
                    CHECK(
                        paths.get_predecessor(vertices_init[i]) ==
                        expected_predecessors[directed][i]
                    );
                }
            }

            for (const auto& [idx, v] : std::views::enumerate(paths.get_path(vertices_init[8]))) {
                CAPTURE(idx, v, expected_path_to_8[directed][idx]);
                CHECK((v == expected_path_to_8[directed][idx]));
            }

            for (const auto& [idx, v] : std::views::enumerate(paths.get_path(vertices_init[5]))) {
                CAPTURE(idx, v, expected_path_to_8[directed][idx]);
                CHECK((v == expected_path_to_5[directed][idx]));
            }
        }
    }
}

TEMPLATE_TEST_CASE(
    "Graphs - A*",
    "[graphs-algos]",
    UndirectedDenseGraph,
    UndirectedSparseGraph,
    DirectedDenseGraph,
    DirectedSparseGraph
) {
    int n_vertices = 12;

    std::vector<TestVertex> vertices_init{{generate_test_vertices_list(n_vertices)}};
    std::vector<TestEdge> edges_init = {
        {{0}, {1}, 4},  {{0}, {7}, 8},   {{1}, {2}, 8}, {{1}, {7}, 11}, {{2}, {3}, 7},
        {{2}, {5}, 4},  {{2}, {8}, 2},   {{3}, {4}, 9}, {{3}, {5}, 14}, {{4}, {5}, 10},
        {{5}, {6}, 2},  {{6}, {7}, 1},   {{6}, {8}, 6}, {{7}, {8}, 7},  {{9}, {10}, 4},
        {{9}, {11}, 2}, {{11}, {10}, 1},
    };

    TestType g(n_vertices);
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    using Path = std::vector<std::tuple<TestVertex, weight_t>>;

    std::vector<std::tuple<
        // start
        TestVertex,
        // end
        TestVertex,
        // path = vector<vertex, distance from start>
        std::vector<std::tuple<TestVertex, weight_t>>>>
        expected[2] = {
            {
                {{0}, {8}, {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{8}, 14}}},
                {{0}, {5}, {{{0}, 0}, {{7}, 8}, {{6}, 9}, {{5}, 11}}},
                {{4}, {2}, {{{4}, 0}, {{5}, 10}, {{2}, 14}}},
                {{0}, {11}, {}},
            },
            {
                {{0}, {8}, {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{8}, 14}}},
                {{0}, {5}, {{{0}, 0}, {{1}, 4}, {{2}, 12}, {{5}, 16}}},
                {{4}, {2}, {}},
                {{0}, {11}, {}},
            }
        };

    bool directed = g.is_directed();
    for (const auto& [start, end, expected_path] : expected[directed]) {
        CAPTURE(start, end, directed);
        // build our cost function from the real distances
        auto paths_to_end = g::all_shortest_paths(g, end);
        auto cost_function = [&paths_to_end](const TestVertex& i, const TestVertex& /* j */) {
            return paths_to_end.get_distance(i);
        };

        auto path = g::shortest_path(g, start, end, cost_function);
        auto v_path = path.get_path() | std::ranges::to<std::vector>();
        CHECK(v_path == expected_path);
    }
}

TEMPLATE_TEST_CASE("Graphs - Max Flow", "[graphs-algos]", DirectedDenseGraph, DirectedSparseGraph) {
    int n_vertices = 6;
    std::vector<TestVertex> vertices_init{{generate_test_vertices_list(n_vertices)}};
    std::vector<TestEdge> edges_init = {
        {{0}, {1}, 16}, {{0}, {2}, 13}, {{1}, {2}, 10}, {{1}, {3}, 12}, {{2}, {1}, 4},
        {{2}, {4}, 14}, {{3}, {2}, 9},  {{3}, {5}, 20}, {{4}, {3}, 7},  {{4}, {5}, 4},
    };

    TestType g(n_vertices);
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    auto max_flow = g::maximum_flow(g, {0}, {5});
    CAPTURE(max_flow);
    CHECK(max_flow == 23);
}

TEMPLATE_TEST_CASE(
    "Graphs - Bridges",
    "[graphs-algos]",
    UndirectedDenseGraph,
    UndirectedSparseGraph
) {
    int n_vertices = 13;
    std::vector<TestVertex> vertices_init{{generate_test_vertices_list(n_vertices)}};
    std::vector<TestEdge> edges_init = {
        {{0}, {1}}, {{0}, {2}}, {{1}, {2}}, {{1}, {4}},  {{2}, {3}},   {{3}, {7}},  {{4}, {5}},
        {{5}, {6}}, {{6}, {4}}, {{7}, {8}}, {{9}, {10}}, {{10}, {11}}, {{11}, {9}}, {{11}, {12}},
    };

    std::unordered_set<TestEdge> expected_bridges = {
        {{1}, {4}}, {{4}, {1}}, {{2}, {3}}, {{3}, {2}},   {{3}, {7}},
        {{7}, {3}}, {{7}, {8}}, {{8}, {7}}, {{11}, {12}}, {{12}, {11}},
    };

    TestType g(n_vertices);
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    std::unordered_set<TestEdge> result;
    for (auto [start, end, _] : g::bridges(g)) {
        result.emplace(start, end);
        result.emplace(end, start);
    }

    CAPTURE(expected_bridges.size(), result.size());
    CHECK(expected_bridges == result);
}

TEMPLATE_TEST_CASE(
    "Graphs - Articulation Points",
    "[graphs-algos]",
    UndirectedDenseGraph,
    UndirectedSparseGraph
) {
    int n_vertices = 14;
    std::vector<TestVertex> vertices_init{{generate_test_vertices_list(n_vertices)}};
    std::vector<TestEdge> edges_init = {
        {{0}, {1}},  {{0}, {2}},   {{1}, {2}},  {{2}, {3}},   {{2}, {4}},   {{3}, {4}}, {{4}, {5}},
        {{4}, {6}},  {{4}, {7}},   {{4}, {9}},  {{4}, {10}},  {{5}, {6}},   {{5}, {6}}, {{7}, {8}},
        {{9}, {10}}, {{10}, {11}}, {{11}, {9}}, {{11}, {12}}, {{12}, {13}},
    };

    std::unordered_set<TestVertex> expected_articulation_points = {
        {2}, {4}, {7}, {11}, {12},
    };

    TestType g(n_vertices);
    g.add_vertices(vertices_init);
    g.set_edges(edges_init);

    std::unordered_set<TestVertex> result;
    for (auto vertex : g::articulation_points(g)) {
        result.emplace(vertex);
    }

    CAPTURE(expected_articulation_points.size(), result.size());
    CHECK(expected_articulation_points == result);
}

TEMPLATE_TEST_CASE(
    "Graphs - Strongly connected components",
    "[graphs-algos]",
    DirectedDenseGraph,
    DirectedSparseGraph
) {
    std::vector<std::tuple<int, std::vector<TestEdge>, std::vector<std::unordered_set<TestVertex>>>>
        graph_defs{
            {
                // n vertices
                5,
                // edges
                {{{1}, {0}}, {{0}, {2}}, {{2}, {1}}, {{0}, {3}}, {{3}, {4}}},
                // expected SCCs
                {{{4}}, {{3}}, {{1}, {2}, {0}}},
            },
            {
                4,
                {{{0}, {1}}, {{1}, {2}}, {{2}, {3}}},
                {{{0}}, {{1}}, {{2}}, {{3}}},
            },
            {
                7,
                {{{0}, {1}},
                 {{1}, {2}},
                 {{2}, {0}},
                 {{1}, {3}},
                 {{1}, {4}},
                 {{1}, {6}},
                 {{3}, {5}},
                 {{4}, {5}}},
                {{{5}}, {{4}}, {{3}}, {{6}}, {{2}, {1}, {0}}},
            },
            {
                11,
                {{{0}, {1}},
                 {{0}, {3}},
                 {{1}, {2}},
                 {{1}, {4}},
                 {{2}, {0}},
                 {{2}, {6}},
                 {{3}, {2}},
                 {{4}, {5}},
                 {{4}, {6}},
                 {{5}, {6}},
                 {{5}, {7}},
                 {{5}, {8}},
                 {{5}, {9}},
                 {{6}, {4}},
                 {{7}, {9}},
                 {{8}, {9}},
                 {{9}, {8}}},
                {{{8}, {9}}, {{7}}, {{5}, {4}, {6}}, {{3}, {2}, {1}, {0}}, {{10}}},
            },
            {
                5,
                {{{0}, {1}}, {{1}, {2}}, {{2}, {3}}, {{2}, {4}}, {{3}, {0}}, {{4}, {2}}},
                {{{4}, {3}, {2}, {1}, {0}}},
            },
        };
    for (const auto& [n_vertices, edges, expected] : graph_defs) {
        TestType g(n_vertices);
        g.set_edges(edges, true);
        // add vertices without edges
        for (unsigned i = g.size(); i < n_vertices; ++i) {
            g.add_vertex({{i}});
        }

        std::vector<std::unordered_set<TestVertex>> sccs;
        for (const auto& scc : g::strongly_connected_components(g)) {
            sccs.push_back(std::unordered_set(scc.begin(), scc.end()));
        }
        CAPTURE(n_vertices);
        CAPTURE(sccs, expected);
        REQUIRE(sccs.size() == expected.size());
        for (const auto& scc : sccs) {
            CAPTURE(scc);
            CHECK(std::ranges::find(expected, scc) != expected.end());
        }
    }
}
