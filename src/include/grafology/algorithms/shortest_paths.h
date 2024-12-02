#pragma once
#include "../graph.h"
#include <queue>

namespace grafology {

    /**
     * @brief This struct allows to process the results of the algorithm shortest_paths
     */
    struct ShortestPathsImpl {
        static constexpr weight_t D_INFINITY = std::numeric_limits<weight_t>::max();
        static constexpr vertex_t NO_PREDECESSOR = std::numeric_limits<vertex_t>::max();

        ShortestPathsImpl(size_t n_vertices, vertex_t start)
        : _distances(n_vertices, D_INFINITY), 
          _predecessors(n_vertices, NO_PREDECESSOR),
          _start(start) {}

        std::vector<weight_t> _distances;
        std::vector<vertex_t> _predecessors;
        const vertex_t _start;

        auto size() const {
            return _distances.size();
        }

        bool is_reachable(vertex_t end) const {
            return _distances[end] != D_INFINITY;
        }

        std::vector<vertex_t> get_path(vertex_t end) const {
            std::vector<vertex_t> path;
            if (_distances[end] == D_INFINITY) {
                return path;
            }
            auto current = end;
            while (current != _start) {
                path.push_back(current);
                current = _predecessors[current];
            }
            path.push_back(_start);
            std::reverse(path.begin(), path.end());
            return path;
        }
    };

    /**
     * @brief Compute the shortest paths from a vertex to all other vertices in a graph
     * @remark this is based on the Dijkstra's algorithm
     */
    template <GraphImpl G>
    ShortestPathsImpl shortest_paths(const G& graph, vertex_t start) {
        if (graph.is_directed()) {
            throw error("Shortest paths works only on undirected graphs");
        }

        const auto n_vertices = graph.size();
        ShortestPathsImpl res(n_vertices);
        std::priority_queue<std::pair<weight_t, vertex_t>> pq;

        res.distances[start] = 0;
        pq.push({0, start});
        while (!pq.empty()) {
            if (pq.empty()) {
                break;
            }
            auto [d, v] = pq.top();
            pq.pop();
            if (d > res.distances[v]) {
                continue;
            }
            for (const auto& edge : graph.get_neighbors(v)) {
                auto new_d = d + edge.weight;
                if (new_d < res.distances[edge.end]) {
                    res.distances[edge.end] = new_d;
                    res.predecessors[edge.end] = v;
                    pq.push({new_d, edge.end});
                }
            }
        }
        return res;
    }

    /**
     * @brief Compute the shortest paths from a vertex to all other vertices in a graph
     * @remark this is based on the Dijkstra's algorithm
     */
    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected>
    struct ShortestPaths {
        ShortestPaths(ShortestPathsImpl&& shortest_paths, const Graph<Impl, Vertex, IsDirected>& graph)
        : ShortestPathsImpl(std::move(shortest_paths)), graph(graph)
        {}

        auto size() const {
            return _shortest_paths.distances.size();
        }

        bool is_reachable(const Vertex& v) const {
            return _shortest_paths.distances[graph.get_internal_index(v)] != ShortestPathsImpl::D_INFINITY;
        }

        weight_t distance(const Vertex& v) const {
            return _shortest_paths.distances[graph.get_internal_index(v)];
        }

        const Vertex& predecessor(const Vertex& v) const {
            return graph.get_vertex(_shortest_paths.predecessors[graph.get_internal_index(v)]);
        }

        generator<Vertex> get_path(const Vertex& v) const {
            auto sp = _shortest_paths.get_path(graph.get_internal_index(v));
            for (auto v : sp) {
                co_yield graph.get_vertex_from_internal_index(v);
            }
        }

    private:
        const ShortestPathsImpl _shortest_paths;
        const Graph<Impl, Vertex, IsDirected>& graph;
    };

    template<GraphImpl Impl, VertexKey Vertex>
    ShortestPaths<Impl, Vertex, false> shortest_paths(const Graph<Impl, Vertex, false>& graph, const Vertex& start) {
        auto sp_impl = shortest_paths(graph.impl(), graph.get_internal_index(start));
        return ShortestPaths<GraphImpl, VertexKey, true>(graph, std::move(sp_impl));
    }

    template<GraphImpl Impl, VertexKey Vertex>
    ShortestPaths<Impl, Vertex, true> shortest_paths(const Graph<Impl, Vertex, true>& graph, const Vertex& start) {
        static_assert(false, "Shortest paths works only on undirected graphs");
    }
}

