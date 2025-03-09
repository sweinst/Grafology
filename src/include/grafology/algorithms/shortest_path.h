#pragma once
#include <functional>
#include <queue>
#include "requirements.h"

namespace grafology {
  /**
   * @brief This struct allows to process the results of the algorithm all_shortest_paths
   * @remark it allows to save the results of the algorithm and so avoid to use dangling references
   */
  template <typename weight_t>
  struct ShortestPathsImpl {
    using edge_lt = edge_t<weight_t>;
    static constexpr auto D_INFINITY = edge_lt::D_INFINITY;
    using step_lt = std::tuple<vertex_t, weight_t>;

    ShortestPathsImpl(size_t n_vertices, vertex_t end)
        : _distances(n_vertices, D_INFINITY)
        , _predecessors(n_vertices, NO_PREDECESSOR)
        , _end(end) {
          assert(_end < n_vertices);
        }
    ShortestPathsImpl(ShortestPathsImpl&&) = default;
    ShortestPathsImpl(const ShortestPathsImpl&) = default;
    ShortestPathsImpl& operator=(ShortestPathsImpl&&) = default;
    ShortestPathsImpl& operator=(const ShortestPathsImpl&) = default;

    std::vector<weight_t> _distances;
    std::vector<vertex_t> _predecessors;
    vertex_t _end;

    auto size() const { return _distances.size(); }

    bool is_reachable() const { return _distances[_end] != D_INFINITY; }

    std::vector<step_lt> get_path() const {
      std::vector<step_lt> path;
      if (!is_reachable()) {
        return path;
      }
      auto current = _end;
      do {
        path.push_back(std::make_tuple(current, _distances[current]));
        current = _predecessors[current];
      }
      while (current != NO_PREDECESSOR);
      std::ranges::reverse(path);
      return path;
    }
  };

  /**
   * @brief Compute the shortest path from one vertex to another
   * @return a vector of tuples (vertex, distance from start)
   * @remark this is based on the A* algorithm
   * 
   * @tparam Graph The graph type
   * @tparam F The cost function type
   * 
   * @param g The graph
   * @param start The start vertex
   * @param end The end vertex
   * @param f The cost function
   */
  template <typename Graph, PathCostFunctionImpl F>
  requires GraphImpl<Graph, typename Graph::weight_lt>
  ShortestPathsImpl<typename Graph::weight_lt> shortest_path(const Graph& graph, vertex_t start, vertex_t end, F& f) {
    using weight_lt = typename Graph::weight_lt;

    assert(start < graph.size() && end < graph.size());
    if (graph.is_directed()) {
      throw error("Shortest path works only on undirected graphs");
    }
    const auto n_vertices = graph.size();
    using cost_type = decltype(f(start, end));
    std::priority_queue<std::pair<cost_type, vertex_t>> pq;
    ShortestPathsImpl<weight_lt> res(n_vertices, end);
    std::vector<bool> visited(n_vertices, false);

    res._distances[start] = 0;
    pq.push({f(start, end), start});
    while (!pq.empty()) {
      auto [_, v] = pq.top();
      pq.pop();
      if (v == end) {
        return res;
      }
      auto d = res._distances[v];
      visited[v] = true;
      for (const auto& edge : graph.get_neighbors(v)) {
        if (edge.weight <= 0) {
          throw error("All shortest paths: negative weights are not allowed");
        }
        if (visited[edge.end]) {
          continue;
        }
        auto new_d = d + edge.weight;
        if (new_d < res._distances[edge.end]) {
          res._distances[edge.end] = new_d;
          res._predecessors[edge.end] = v;
          pq.push({f(edge.end, end), edge.end});
        }
      }
    }
    return {n_vertices, end};
  }

  /**
   * @brief This struct allows to process the results of the algorithm all_shortest_paths
   * @remark it allows to save the results of the algorithm and so avoid to use dangling references
   */
  template <typename Impl, VertexKey Vertex, bool IsDirected>
  requires GraphImpl<Impl, typename Impl::weight_lt>
  struct ShortestPaths {
    using weight_lt = typename Impl::weight_lt;
    using step_lt = Step<Vertex, weight_lt>;
    ShortestPaths(
        ShortestPathsImpl<weight_lt>&& shortest_paths,
        const Graph<Impl, Vertex, IsDirected, weight_lt>& graph
    )
        : _shortest_paths(std::move(shortest_paths))
        , graph(graph) {}
    ShortestPaths(const ShortestPaths&) = default;
    ShortestPaths(ShortestPaths&&) = default;
    ShortestPaths& operator=(const ShortestPaths&) = default;
    ShortestPaths& operator=(ShortestPaths&&) = default;

    auto size() const { return _shortest_paths._distances.size(); }

    bool is_reachable() const {
      return _shortest_paths.is_reachable();
    }

    generator<step_lt> get_path() const {
      for (const auto& [v, d]: _shortest_paths.get_path()) {
        co_yield std::make_tuple(graph.get_vertex_from_internal_index(v), d);
      }
    }

   private:
    const ShortestPathsImpl<typename Impl::weight_lt> _shortest_paths;
    const Graph<Impl, Vertex, IsDirected, typename Impl::weight_lt>& graph;
  };

  /**
   * @brief Compute the shortest path from one vertex to another
   * @return a range of tuples (vertex, distance from start)
   * @remark this is based on the A* algorithm
   * 
   * @tparam Graph The graph type
   * @tparam F The cost function type
   * 
   * @param g The graph
   * @param start The start vertex
   * @param end The end vertex
   * @param f The cost function
   */
  template <typename Impl, VertexKey Vertex, PathCostFunction<Vertex> F>
  requires GraphImpl<Impl, typename Impl::weight_lt>
  ShortestPaths<Impl, Vertex, false> shortest_path(const Graph<Impl, Vertex, false, typename Impl::weight_lt>& graph, const Vertex& start, const Vertex& end, F& f) {
    auto cost_function = [&] (vertex_t u, vertex_t v) {
      assert(u < graph.size() && v < graph.size());
      return f(graph.get_vertex_from_internal_index(u), graph.get_vertex_from_internal_index(v));
    };
    assert(graph.get_internal_index(start) != INVALID_VERTEX && graph.get_internal_index(end)  != INVALID_VERTEX);
    auto sp_impl = shortest_path(graph.impl(), graph.get_internal_index(start), graph.get_internal_index(end), cost_function);
    return ShortestPaths<Impl, Vertex, false>(std::move(sp_impl), graph);
  }

  template <typename Impl, VertexKey Vertex, PathCostFunction<Vertex> F>
  requires GraphImpl<Impl, typename Impl::weight_lt>
  ShortestPaths<Impl, Vertex, true> shortest_path(const Graph<Impl, Vertex, true, typename Impl::weight_lt>& graph, const Vertex& start, const Vertex& end, F& f) {
    static_assert(false, "Shortest paths works only on undirected graphs");
  }

}  // namespace grafology
