#pragma once
#include "../graph.h"
#include <queue>

namespace grafology {
    /**
    * @brief Perform a topological sort on a directed graph
    * @remark This is an implementation of Kahn's algorithm
    */
    template <typename G>
    requires GraphImpl<G, typename G::weight_lt>
    generator<std::pair<unsigned, vertex_t>> topological_sort(const G& graph) {
        if (!graph.is_directed()) {
            throw error("Topological sort works only on directed graphs");
        }

        const auto n_vertices = graph.size();
        
        // count the number of incoming edges for each vertex
        std::vector<int> in_degrees(n_vertices, 0);
        for (auto edge : graph.get_all_edges()) {
            ++in_degrees[edge.end];
        }

        unsigned n_processed = 0;
        unsigned i_group = 0;
        std::vector<vertex_t> to_process;
        while (true) {
            if (n_processed == n_vertices) {
                break;
            }
            // find the roots (vertices with no incoming edges)
            to_process.clear();
            for (vertex_t i = 0; i < n_vertices; ++i) {
                if (in_degrees[i] == 0) {
                    to_process.push_back(i);
                }
            }
            if (to_process.empty()) {
                throw error("The graph has a cycle");
            }
            for (auto vertex : to_process) {
                // return the new root and decease the in-degree of its neighbors
                co_yield std::make_pair(i_group, vertex);
                ++n_processed;
                in_degrees[vertex] = -1;
                for (auto neighbour : graph.get_raw_neighbors(vertex)) {
                    --in_degrees[neighbour];
                }
            }
            ++i_group;
        }

    }

    /**
    * @brief Perform a topological sort on a directed graph
    * @remark This is an implementation of Kahn's algorithm
    */
    template<typename Impl, VertexKey Vertex>
    requires GraphImpl<Impl, typename Impl::weight_lt>
    generator<std::pair<unsigned, Vertex>> topological_sort(const Graph<Impl, Vertex, true, typename Impl::weight_lt>& graph) {
        const auto impl = graph.impl();
        for (auto [group, vertex] : topological_sort(impl)) {
            co_yield std::make_pair(group, graph.get_vertex_from_internal_index(vertex));
        }
    }

    /**
    * @brief Prevent topological sorts on undirected graphs
    */
    template<typename Impl, VertexKey Vertex>
    requires GraphImpl<Impl, typename Impl::weight_lt>
    generator<std::pair<unsigned, Vertex>> topological_sort(const Graph<Impl, Vertex, false, typename Impl::weight_lt>& graph) {
        static_assert(false, "Topological sort works only on directed graphs");
    }

} // namespace grafology
