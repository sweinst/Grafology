#pragma once
#include "../grafology.h"
#include <queue>

namespace grafology {
    template <GraphImpl G>
    generator<std::pair<unsigned, node_t>> topological_sort(const G& graph) {
        if (!graph.is_directed()) {
            throw error("Topological sort works only on directed graphs");
        }

        const auto n_nodes = graph.size();
        
        // count the number of incoming edges for each vertex
        std::vector<int> in_degrees(n_nodes, 0);
        for (node_t i = 0; i < n_nodes; ++i) {
            for (auto edge : graph.get_neighbors(i)) {
                ++in_degrees[edge.end];
            }
        }

        unsigned n_processed = 0;
        unsigned i_group = 0;
        std::vector<node_t> to_process;
        while (true) {
            if (n_processed == n_nodes) {
                break;
            }
            // find the roots (vertices with no incoming edges)
            to_process.clear();
            for (node_t i = 0; i < n_nodes; ++i) {
                if (in_degrees[i] == 0) {
                    to_process.push_back(i);
                }
            }
            if (to_process.empty()) {
                throw error("The graph has a cycle");
            }
            for (auto node : to_process) {
                // return the new root and decease the in-degree of its neighbors
                co_yield std::make_pair(i_group, node);
                ++n_processed;
                in_degrees[node] = -1;
                for (auto neighbour : graph.get_raw_neighbors(node)) {
                    --in_degrees[neighbour];
                }
            }
            ++i_group;
        }

    }

/*     template <GraphImpl Impl> 
    generator<node_t> topological_sort(const Impl& graph) {
        std::vector<unsigned> in_degrees(graph.size(), 0);
        for (node_t i = 0; i < graph.size(); ++i) {
            for (auto edge : graph.get_neighbors(i)) {
                ++in_degrees[edge.end];
            }
        }

        std::queue<node_t> q;
        for (node_t i = 0; i < graph.size(); ++i) {
            if (in_degrees[i] == 0) {
                q.push(i);
            }
        }

        while (!q.empty()) {
            auto node = q.front();
            q.pop();
            co_yield node;
            for (auto edge : graph.get_neighbors(node)) {
                --in_degrees[edge.end];
                if (in_degrees[edge.end] == 0) {
                    q.push(edge.end);
                }
            }
        }
    }
 */    
} // namespace grafology
