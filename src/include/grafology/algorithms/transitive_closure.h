#pragma once
#include "../graph.h"

namespace grafology {
    /**
     * @brief Compute the transitive closure of a graph
     * @tparam G the type of the graph
     * @details The transitive closure of a graph is a graph which will add an edge from vertex i to vertex j if they are indirectly linked
     * So that if there is a path from i to j, and there is an edge from j to k, there will be an edge from i to k
     * @remark The algorithm is derived from the Warshall algorithm. 
     * @remark Its complexity is O(n^3) where n is the number of vertices for a densely connected graph 
     * but tends to O(n^2) as the graph becomes sparser.
     */
    // TODO: should we put directly this code in the graph class? Especially when using an adjacency matrix?
    // TODO: investigate the use of symmetry for undirected graphs
    // TODO: investigate faster algorithms
    template <GraphImpl G>
    void transitive_closure(G& graph) {
        const auto n_vertices = graph.size();
        for (vertex_t k = 0; k < n_vertices; ++k) {
            for (vertex_t i = 0; i < n_vertices; ++i) {
                if (graph.has_edge(i, k)) {
                    for (vertex_t j = 0; j < n_vertices; ++j) {
                        if (graph.has_edge(k, j)) {
                            graph.set_edge(i, j, 1);
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Compute the transitive closure of a graph
     * @tparam Impl the type of the graph implementation
     * @tparam Vertex the type of the vertex key
     * @tparam IsDirected true if the graph is directed
     * @details The transitive closure of a graph is a graph which will add an edge from vertex i to vertex j if they are indirectly linked
     * So that if there is a path from i to j, and there is an edge from j to k, there will be an edge from i to k
     * @remark The algorithm is derived from the Floyd-Warshall algorithm. 
     * @remark Its complexity is O(n^3) where n is the number of vertices for a densely connected graph 
     * but tends to O(n^2) as the graph becomes sparser.
     */
    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected>
    void transitive_closure(Graph<Impl, Vertex, IsDirected>& graph) {     
        transitive_closure(graph.get_impl());
    }

} // namespace grafology
