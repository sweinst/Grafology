#include <catch2/catch_template_test_macros.hpp>
#include <grafology/dense_graph_impl.h>
#include <grafology/sparse_graph_impl.h>
#include <ranges>
#include <set>

namespace g = grafology;

TEMPLATE_TEST_CASE("Graph implementations", "[graph-impl]", 
    g::DenseGraphImpl , g::SparseGraphImpl)
{
    constexpr unsigned max_vertices = 11;
    constexpr unsigned n_vertices = 7;
    const std::vector<g::edge_t> edges_init {
        {0, 1, 1},
        {0, 2, 2},
        {2, 3, 5},
        {2, 4, 6},
        {2, 5, 7},
        {3, 1, 4},
        {5, 1, 6},
    };
    constexpr unsigned n_extra_vertices = 4;
    const std::vector<g::edge_t> extra_edges_init {
        {3, 10, 13},
        {5, 8, 13},
        {6, 7, 13},
        {8, 3, 11},
        {8, 7, 15},
        {8, 9, 17},
        {10, 10, 20},
    };

    for (bool is_directed: {true, false})
    {
        CAPTURE(is_directed);
        //initialization
        std::vector<unsigned> degrees = is_directed ?  
            std::vector<unsigned>{ 
                2, 0, 3, 1, 0, 
                1 , 0} :
            std::vector<unsigned>{
                2, 3, 4, 2, 1, 
                2};

        TestType g(max_vertices, n_vertices, is_directed);
        g.set_edges(edges_init);
        for (const auto [idx, degree]: std::views::enumerate(degrees))
        {
            CAPTURE(idx, degree, g.degree(idx));
            CHECK(g.degree(idx) == degree);
        }

        // modifications
        std::vector<unsigned> extra_degrees = is_directed ?
            std::vector<unsigned>{ 
                2, 0, 3, 2, 0, 
                2, 1, 0, 3, 0, 
                1 } :
            std::vector<unsigned>{
                2, 3, 4, 4, 1, 
                3, 1, 2, 4, 1, 
                2 };

        for (auto _ : g.add_vertices(n_extra_vertices)) {};
        CAPTURE(g.size(), n_vertices + n_extra_vertices);
        REQUIRE(g.size() == n_vertices + n_extra_vertices);
        for (const auto& edge: extra_edges_init)
        {
            g.set_edge(edge);
        }

        for (const auto [idx, degree]: std::views::enumerate(extra_degrees))
        {
            CAPTURE(idx, degree, g.degree(idx));
            CHECK(g.degree(idx) == degree);
        }

        // neighbors
        std::vector<std::set<g::node_t>> neighbors(g.size());
        auto edges = {edges_init, extra_edges_init};
        for(const auto& edge: std::views::join(edges))
        {
            if (edge.start != edge.end)
            {                
                neighbors[edge.start].insert(edge.end);
                if (!is_directed)
                {
                    neighbors[edge.end].insert(edge.start);
                }
            }
        }

        for (g::node_t i = 0; i < g.size(); i++)
        {
            unsigned n_neighbors = 0;
            for (const auto& neighbor: g.get_neighbors(i))
            {
                CAPTURE(neighbor.start, neighbor.end, neighbor.weight);
                CHECK(neighbors[i].contains(neighbor.end));
                CHECK(neighbor.start + neighbor.end == neighbor.weight);
                ++n_neighbors;
            }
            CAPTURE(neighbors[i].size(), n_neighbors);
            CHECK(neighbors[i].size() == n_neighbors);
        }
    }
}

