#pragma once
#include <queue>
#include "../graph.h"

namespace grafology {
  /**
   * @brief This struct allows to process the results of the algorithm all_shortest_paths
   * @remark it allows to save the results of the algorithm and so avoid to use dangling references
   */
  struct AllShortestPathsImpl {
    AllShortestPathsImpl(size_t n_vertices, vertex_t start)
        : _distances(n_vertices, D_INFINITY)
        , _predecessors(n_vertices, NO_PREDECESSOR)
        , _start(start) {}
    AllShortestPathsImpl(AllShortestPathsImpl&&) = default;
    AllShortestPathsImpl(const AllShortestPathsImpl&) = default;
    AllShortestPathsImpl& operator=(AllShortestPathsImpl&&) = default;
    AllShortestPathsImpl& operator=(const AllShortestPathsImpl&) = default;

    std::vector<weight_t> _distances;
    std::vector<vertex_t> _predecessors;
    vertex_t _start;

    auto size() const { return _distances.size(); }

    bool is_reachable(vertex_t end) const { 
      assert(end < _distances.size());
      return _distances[end] != D_INFINITY; 
    }

    std::vector<step_t> get_path(vertex_t end) const {
      assert(end < _distances.size());
      std::vector<step_t> path;
      if (_distances[end] == D_INFINITY) {
        return path;
      }
      auto current = end;
      while (current != _start) {
        path.push_back(std::make_tuple(current, _distances[current]));
        current = _predecessors[current];
      }
      path.push_back(std::make_tuple(_start, 0));
      std::ranges::reverse(path);
      return path;
    }
  };

  /**
   * @brief Compute the shortest paths from a vertex to all other vertices in a
   * graph
   * @remark this is based on the Dijkstra's algorithm
   */
  template <GraphImpl G>
  AllShortestPathsImpl all_shortest_paths(const G& graph, vertex_t start) {
    assert(start < graph.size());

    if (graph.is_directed()) {
      throw error("Shortest paths works only on undirected graphs");
    }

    const auto n_vertices = graph.size();
    AllShortestPathsImpl res(n_vertices, start);
    std::priority_queue<std::pair<weight_t, vertex_t>> pq;

    res._distances[start] = 0;
    pq.push({0, start});
    while (!pq.empty()) {
      if (pq.empty()) {
        break;
      }
      auto [d, v] = pq.top();
      pq.pop();
      if (d > res._distances[v]) {
        continue;
      }
      for (const auto& edge : graph.get_neighbors(v)) {
        if (edge.weight <= 0) {
          throw error("All shortest paths: negative weights are not allowed");
        }
        auto new_d = d + edge.weight;
        if (new_d < res._distances[edge.end]) {
          res._distances[edge.end] = new_d;
          res._predecessors[edge.end] = v;
          pq.push({new_d, edge.end});
        }
      }
    }
    return res;
  }

  /**
   * @brief Compute the shortest paths from a vertex to all other vertices in a
   * graph
   * @remark this is based on the Dijkstra's algorithm
   */
  template <GraphImpl Impl, VertexKey Vertex, bool IsDirected>
  struct AllShortestPaths {
    AllShortestPaths(
        AllShortestPathsImpl&& shortest_paths,
        const Graph<Impl, Vertex, IsDirected>& graph
    )
        : _shortest_paths(std::move(shortest_paths))
        , graph(graph) {}
    AllShortestPaths(const AllShortestPaths&) = default;
    AllShortestPaths(AllShortestPaths&&) = default;
    AllShortestPaths& operator=(const AllShortestPaths&) = default;
    AllShortestPaths& operator=(AllShortestPaths&&) = default;

    auto size() const { return _shortest_paths._distances.size(); }

    bool is_reachable(const Vertex& v) const {
      assert(graph.get_internal_index(v) != INVALID_VERTEX);
      return _shortest_paths._distances[graph.get_internal_index(v)] != D_INFINITY;
    }

    weight_t get_distance(const Vertex& v) const {
      assert(graph.get_internal_index(v) != INVALID_VERTEX);
      return _shortest_paths._distances[graph.get_internal_index(v)];
    }

    const Vertex& get_predecessor(const Vertex& v) const {
      assert(graph.get_internal_index(v) != INVALID_VERTEX);
      return graph.get_vertex_from_internal_index(
          _shortest_paths._predecessors[graph.get_internal_index(v)]
      );
    }

    generator<Step<Vertex>> get_path(const Vertex& v) const {
      assert(graph.get_internal_index(v) != INVALID_VERTEX);
      auto sp = _shortest_paths.get_path(graph.get_internal_index(v));
      for (const auto& [v, d] : sp) {
        co_yield std::make_tuple(graph.get_vertex_from_internal_index(v), d);
      }
    }

   private:
    const AllShortestPathsImpl _shortest_paths;
    const Graph<Impl, Vertex, IsDirected>& graph;
  };

  template <GraphImpl Impl, VertexKey Vertex>
  AllShortestPaths<Impl, Vertex, false>
  all_shortest_paths(const Graph<Impl, Vertex, false>& graph, const Vertex& start) {
    assert(graph.get_internal_index(start) != INVALID_VERTEX);
    auto sp_impl = all_shortest_paths(graph.impl(), graph.get_internal_index(start));
    return AllShortestPaths<Impl, Vertex, false>(std::move(sp_impl), graph);
  }

  template <GraphImpl Impl, VertexKey Vertex>
  AllShortestPaths<Impl, Vertex, true>
  all_shortest_paths(const Graph<Impl, Vertex, true>& graph, const Vertex& start) {
    static_assert(false, "Shortest paths works only on undirected graphs");
  }
}  // namespace grafology
