#pragma once
#include "all_shortest_paths.h"

namespace grafology {

    /**
     * @brief Find one or no negative cycles starting at each vertex in a graph
     * @remark this is based on the Bellman-Ford algorithm
     */
    template <typename G>
        requires GraphImpl<G, typename G::weight_lt>
    generator<std::vector<vertex_t>> find_all_negative_cycles(const G& graph, vertex_t source) {
        if (!graph.is_directed()) {
            throw std::runtime_error("Bellman-Ford algorithm only works with directed graphs");
        }

        using weight_lt = typename G::weight_lt;
        static constexpr auto D_INFINITY = edge_t<weight_lt>::D_INFINITY;
        auto n = graph.size();
        AllShortestPathsImpl<weight_lt> res(n, static_cast<vertex_t>(0));

        std::vector<bool> in_cycle(n, false);
        res._distances[source] = 0;
        // do n-1 relaxations
        for (auto step = 0; step != n - 1; ++step) {
            for (const auto edge : graph.get_all_edges()) {
                if (res._distances[edge.start] != D_INFINITY) {
                    auto new_d = res._distances[edge.start] + edge.weight;
                    if (new_d < res._distances[edge.end]) {
                        res._distances[edge.end] = new_d;
                        res._predecessors[edge.end] = edge.start;
                    }
                }
            }
        }
        // check for negative cycles
        for (const auto edge : graph.get_all_edges()) {
            if (res._distances[edge.start] != D_INFINITY) {
                auto new_d = res._distances[edge.start] + edge.weight;
                if (new_d < res._distances[edge.end]) {
                    std::vector<bool> visited(n, false);
                    visited[edge.start] = true;
                    visited[edge.end] = true;
                    std::vector<vertex_t> cycle{ edge.end, edge.start };
                    auto u = res._predecessors[edge.start];
                    while (!visited[u]) {
                        visited[u] = true;
                        cycle.push_back(u);
                        u = res._predecessors[u];
                    }
                    if (cycle.size() <= 2) {
                        continue;
                    }
                    cycle.push_back(edge.end);
                    std::ranges::reverse(cycle);
                    
                    co_yield cycle;
                }
            }
        }
    }
}  // namespace grafology
