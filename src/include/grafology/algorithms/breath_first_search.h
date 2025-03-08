#pragma once
#include "../graph.h"
#include <queue>

namespace grafology {
    template <GraphImpl G>
    generator<vertex_t> breath_first_search(const G& graph, vertex_t start) {
        assert(start < graph.size());
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

    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected, Number weight_t>
    generator<Vertex> breath_first_search(const Graph<Impl, Vertex, IsDirected, weight_t>& graph, const Vertex& start) {
        assert(graph.get_internal_index(start) != INVALID_VERTEX);
        const auto impl = graph.impl();
        auto idx_start = graph.get_internal_index(start);
        for (auto vertex : breath_first_search(impl, idx_start)) {
            co_yield graph.get_vertex_from_internal_index(vertex);
        }
    }
} // namespace grafology
