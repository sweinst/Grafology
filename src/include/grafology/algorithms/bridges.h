#pragma once
#include <stack>
#include "../graph.h"

/**
 * @brief returns the bridges of a graph
 * @remark a bridge is an edge that if removed would increase the number of connected components
 * @remark this is based on the Tarjan's algorithm
 * @tparam G the type of the graph implementation
 */
namespace grafology {
    template <typename G>
    requires GraphImpl<G, typename G::weight_lt>
    generator<typename G::edge_lt> bridges(const G& graph) {
        using edge_lt = typename G::edge_lt;

        if (graph.is_directed()) {
            throw error("Bridges works only on undirected graphs");
        }
        constexpr int UNDEFINED = -1;
        auto V = graph.size();
        std::vector<int> discovery_time(V, UNDEFINED);
        std::vector<int> lowest_time(V, UNDEFINED);
        std::vector<vertex_t> parent(V, NO_PREDECESSOR);
        int timer = 0;

        for (int u = 0; u < V; ++u) {
            if (discovery_time[u] == UNDEFINED) {
                // This is a non-recursive version of Tarjan's algorithm
                // To achieve it, one needs to process a node in 3 phases:
                // - first visit the node (initialize the lowest time and the discovery time)
                // - continue the DFS
                // - post-process the node (update the lowest time of the parent)
                // So when a node need to be processed, one pushes on the stack:
                //    (node, pre-process=False)
                //    (node, pre-process=True)
                // the first pushed value will only be used when the DFS from node is finished
                std::stack<std::pair<vertex_t, bool>> stack;
                stack.push({u, true});
                int time = 0;

                while (!stack.empty()) {
                    auto [v, pre_process] = stack.top();
                    stack.pop();
                    if (pre_process) {
                        auto p = parent[v];
                        discovery_time[v] = lowest_time[v] = time;
                        time++;
                        for (auto w : graph.get_raw_neighbors(v)) {
                            if (discovery_time[w] == UNDEFINED) {
                                parent[w] = v;
                                stack.push({w, false});
                                stack.push({w, true});
                            } else if (w != p) {
                                lowest_time[v] = std::min(lowest_time[v], discovery_time[w]);
                            }
                        }
                    } else {
                        // v has been updated, so one can now update the parent
                        auto p = parent[v];
                        if (p != NO_PREDECESSOR) {
                            lowest_time[p] = std::min(lowest_time[p], lowest_time[v]);
                            if (lowest_time[v] > discovery_time[p]) {
                                co_yield edge_lt{p, v};
                            }
                        }
                    }
                }
            }
        }
    }

    template<typename Impl, VertexKey Vertex>
    requires GraphImpl<Impl, typename Impl::weight_lt>
    generator<EdgeDefinition<Vertex, typename Impl::weight_lt>> bridges(const Graph<Impl, Vertex, false, typename Impl::weight_lt>& graph) {
        for (auto edge : bridges(graph.impl())) {
            co_yield EdgeDefinition<Vertex, typename Impl::weight_lt>{graph.get_vertex_from_internal_index(edge.start), graph.get_vertex_from_internal_index(edge.end), edge.weight};
        }
    }

    template<typename Impl, VertexKey Vertex>
    requires GraphImpl<Impl, typename Impl::weight_lt>
    generator<EdgeDefinition<Vertex, typename Impl::weight_lt>> bridges(const Graph<Impl, Vertex, true, typename Impl::weight_lt>& graph) {
        static_assert(false, "Bridges works only on undirected graphs");
    }

}  // namespace grafology
