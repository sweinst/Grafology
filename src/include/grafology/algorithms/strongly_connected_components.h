#pragma once
#include "../graph.h"
#include <stack>

namespace grafology {
    template <GraphImpl G>
    generator<std::vector<vertex_t>> strongly_connected_components(const G& graph) {
        if (!graph.is_directed()) {
            throw error("Strongly_connected_components works only on directed graphs");
        }
        auto V = graph.size();
        constexpr int UNDEFINED = -1;
        std::vector<int> discovery_time(V, UNDEFINED);
        std::vector<int> lowest_time(V, UNDEFINED);
        std::vector<bool> stack_member(V, false);
        std::vector<vertex_t> parent(V, NO_PREDECESSOR);
        int time = 0;

        for (int u = 0; u < V; ++u) {
            if (discovery_time[u] == UNDEFINED) {
                // This is a non-recursive version of Tarjan's algorithm
                // To achieve it, one needs to process a node in 3 phases:
                // - first visit the node (initialize the lowest time and the discovery time)
                // - continue the DFS
                // - post-process the node (update the lowest time of the parent and check if we have a cycle)
                // So when a node need to be processed, one pushes on the stack:
                //    (node, pre-process=False)
                //    (node, pre-process=True)
                // the first pushed value will only be used when the DFS from node is finished
                std::stack<std::pair<vertex_t, bool>> stack;
                std::stack<vertex_t> dfs_stack;
                
                stack.push({u, false});
                stack.push({u, true});
                while (!stack.empty()) {
                    auto [v, pre_process] = stack.top();
                    stack.pop();
                    if (pre_process) {
                        if (discovery_time[v] == UNDEFINED) {
                            dfs_stack.push(v);
                            stack_member[v] = true;
                            discovery_time[v] = lowest_time[v] = time;
                            ++time;
                            for (auto w : graph.get_raw_neighbors(v)) {
                                if (discovery_time[w] == UNDEFINED)  {
                                    parent[w] = v;
                                    stack.push({w, false});
                                    stack.push({w, true});
                                } else if (stack_member[w]) {
                                    /// it's a back edge
                                    lowest_time[v] = std::min(lowest_time[v], discovery_time[w]);
                                }
                            }
                        }
                    } else {
                        // update the parent
                        auto p = parent[v];
                        if (p != NO_PREDECESSOR) {
                            //  Check if the subtree rooted with u has a connection to one of the ancestors of p
                            lowest_time[p] = std::min(lowest_time[p], lowest_time[v]);
                        }
                        // at this stage, all the children vertices has been processed
                        if (discovery_time[v] == lowest_time[v]) {
                            // A vertex starting an SCC has been found
                            std::vector<vertex_t> component;
                            vertex_t w = NO_PREDECESSOR;
                            while (w != v) {
                                w = dfs_stack.top();
                                dfs_stack.pop();
                                stack_member[w] = false;
                                component.push_back(w);
                            }
                            co_yield component;
                        }
                    }
                }
            }
        }
    }

    template<GraphImpl Impl, VertexKey Vertex>
    generator<std::vector<Vertex>> strongly_connected_components(const Graph<Impl, Vertex, true>& graph) {
        for (auto scc : strongly_connected_components(graph.impl())) {
            auto vertices = scc | 
                std::views::transform([&](auto v) { return graph.get_vertex_from_internal_index(v); }) | 
                std::ranges::to<std::vector<Vertex>>();
            co_yield vertices;
        }
    }

    template<GraphImpl Impl, VertexKey Vertex>
    generator<std::vector<Vertex>> strongly_connected_components(const Graph<Impl, Vertex, false>& graph) {
        static_assert(false, "Bridges works only on directed graphs");
    }

} // namespace grafology

