#pragma once
#include "depth_first_search.h"

namespace grafology {
    /**
     * @brief Compute the maximum flow of a graph. Edges weights are used as capacities.
     * @tparam G The graph implementation
     * @param graph The graph to compute the maximum flow from
     * @param start The source vertex
     * @param end The sink vertex
     * @return The maximum flow
     * @remark This is based on the Ford-Fulkerson algorithm
     * @todo Investigate the Push Relabel algorithm which should be faster
     */
    template <GraphImpl G>
    weight_t maximum_flow(const G& graph, vertex_t start, vertex_t end) {
        assert(start < graph.size() && end < graph.size());
        if(!graph.is_directed()) {
            throw error("maximum_flow: the graph must be directed");
        }

        // the residual graph
        G residual_graph= graph;
        weight_t max_flow = 0;
        DFSPathFinder dfs(residual_graph);
        
        while (true) {
            if (!dfs.find(start, end)) {
                break;
            }
            weight_t path_flow = std::numeric_limits<weight_t>::max();
            for (vertex_t v = end; v != start; v = dfs.get_parent(v)) {
                auto u = dfs.get_parent(v);
                auto w = residual_graph.weight(u, v);
                path_flow = std::min(path_flow, w);
            }
            for (vertex_t v = end; v != start; v = dfs.get_parent(v)) {
                vertex_t u = dfs.get_parent(v);
                residual_graph.set_edge(u, v, residual_graph.weight(u, v) - path_flow);
            }
            max_flow += path_flow;
        }
        return max_flow;
    }

    /**
     * @brief Compute the maximum flow of a graph. Edges weights are used as capacities.
     * @tparam G The graph implementation
     * @param graph The graph to compute the maximum flow from
     * @param start The source vertex
     * @param end The sink vertex
     * @return The maximum flow
     * @remark This is based on the Ford-Fulkerson algorithm
     * @todo Investigate the Push Relabel algorithm which should be faster
     */
    template<GraphImpl Impl, VertexKey Vertex>
    weight_t maximum_flow(const Graph<Impl, Vertex, true>& graph, const Vertex& start, const Vertex& end) {
        assert(graph.get_internal_index(start) != INVALID_VERTEX && graph.get_internal_index(end) != INVALID_VERTEX);
        return maximum_flow(graph.impl(), graph.get_internal_index(start), graph.get_internal_index(end));
    }

    template<GraphImpl Impl, VertexKey Vertex>
   weight_t maximum_flow(const Graph<Impl, Vertex, false>& graph, const Vertex& start, const Vertex& end) {
        static_assert(false, "Maximum flow works only on directed graphs");
        return 0;
    }

} // namespace grafology
