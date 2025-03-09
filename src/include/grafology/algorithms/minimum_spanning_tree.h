#include "../graph.h"
#include "../disjoint_set.h"
#include <ranges>

namespace grafology {
    /**
     * @brief Compute the minimum spanning tree of a graph
     * @tparam G The graph implementation
     * @remark this is based on the Kruskal's algorithm
     */
    template <typename G>
    requires GraphImpl<G, typename G::weight_lt>
    G minimum_spanning_tree(const G& graph) {
        using edge_lt = typename G::edge_lt;
        if (graph.is_directed()) {
            throw error("Minimum spanning tree works only on undirected graphs");
        }
        const auto n_vertices = graph.size();
        // a simplified priority queue
        
        // doesn't work with g++ 14.2, why ? 
        // auto g_edges = graph.get_all_edges();
        // std::vector<edge_lt> edges(g_edges.begin(), g_edges.end());

        std::vector<edge_lt> edges;
        for (auto edge: graph.get_all_edges()) {
            edges.emplace_back(std::move(edge));
        }
        std::sort(edges.begin(), edges.end(), [](const auto& e1, const auto& e2) {
            return e1.weight < e2.weight;
        });
        // the disjoint set data structure
        DisjointSet disjoint_set(n_vertices);

        G res(graph.capacity(), n_vertices, false);
        for (const auto& edge: edges) {
            if (disjoint_set.merge(edge.start, edge.end)) {
                res.set_edge(edge);
            }
        }
        return res;
    }

    /**
     * @brief Compute the minimum spanning tree of an undirected graph
     * @tparam Impl The graph implementation
     * @tparam Vertex The class used for identifying vertices
     * @remark this is based on the Kruskal's algorithm
     */
    template<typename Impl, VertexKey Vertex>
    requires GraphImpl<Impl, typename Impl::weight_lt>
    Graph<Impl, Vertex, false, typename Impl::weight_lt> minimum_spanning_tree(const Graph<Impl, Vertex, false, typename Impl::weight_lt>& graph) {
        auto new_impl = minimum_spanning_tree(graph.impl());
        return Graph<Impl, Vertex, false, typename Impl::weight_lt>(graph, std::move(new_impl));
    }

    template<typename Impl, VertexKey Vertex>
    requires GraphImpl<Impl, typename Impl::weight_lt>
    Graph<Impl, Vertex, true, typename Impl::weight_lt> minimum_spanning_tree(const Graph<Impl, Vertex, true, typename Impl::weight_lt>& graph) {
        static_assert(false, "Minimum spanning tree works only on undirected graphs");
    }

} // namespace grafology
