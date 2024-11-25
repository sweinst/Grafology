#pragma once
#include "../grafology.h"
#include <stack>

namespace grafology {

    /**
     * @brief Perform a depth-first search on a GraphImpl
     * @param graph The graph to search
     * @package start The vertex to start the search from
     */
    template <GraphImpl G>
    generator<vertex_t> depth_first_search(const G& graph, vertex_t start) {
        std::vector<bool> visited(graph.size(), false);
        std::stack<vertex_t> stack;
        stack.push(start);
        while (!stack.empty()) {
            vertex_t current = stack.top();
            stack.pop();
            if (!visited[current]) {
                visited[current] = true;
                co_yield current;
                for (auto neighbour : graph.get_raw_neighbors(current)) {
                    stack.push(neighbour);
                }
            }
        }
    }

    /**
     * @brief Perform a depth-first search on a Graph
     * @param graph The graph to search
     * @package start The vertex to start the search from
     */
    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected>
    generator<Vertex> depth_first_search(const Graph<Impl, Vertex, IsDirected>& graph, const Vertex& start) {
        const auto impl = graph.impl();
        for (auto vertex : depth_first_search(impl, graph.get_internal_index(start))) {
            co_yield graph.get_vertex_from_internal_index(vertex);
        }
    }

} // namespace grafology
