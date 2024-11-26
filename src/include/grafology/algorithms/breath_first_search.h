#pragma once
#include "../graph.h"
#include <queue>

namespace grafology {
    template <GraphImpl G>
    generator<vertex_t> breath_first_search(const G& graph, vertex_t start) {
        std::vector<bool> visited(graph.size(), false);
        std::queue<vertex_t> queue;
        queue.push(start);
        while (!queue.empty()) {
            vertex_t current = queue.front();
            queue.pop();
            if (!visited[current]) {
                visited[current] = true;
                co_yield current;
                for (auto neighbour : graph.get_raw_neighbors(current)) {
                    queue.push(neighbour);
                }
            }
        }
    }

    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected>
    generator<Vertex> breath_first_search(const Graph<Impl, Vertex, IsDirected>& graph, const Vertex& start) {
        const auto impl = graph.impl();
        auto idx_start = graph.get_internal_index(start);
        if (idx_start == -1) {
            throw error("breath_first_search: Initial vertex '{}' not found", start);
        }
        for (auto vertex : breath_first_search(impl, idx_start)) {
            co_yield graph.get_vertex_from_internal_index(vertex);
        }
    }
} // namespace grafology
