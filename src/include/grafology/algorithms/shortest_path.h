#pragma once
#include <functional>
#include <queue>
#include "requirements.h"

namespace grafology {
  /**
   * @brief Compute the shortest path from one vertex to another
   * @remark this is based on the A* algorithm
   * 
   * @tparam Graph The graph type
   * @tparam F The cost function type
   * @param g The graph
   * @param start The start vertex
   * @param end The end vertex
   * @param f The cost function
   */
  template <GraphImpl Graph, PathCostFunctionImpl F>
  std::vector<vertex_t> shortest_path(const Graph& graph, vertex_t start, vertex_t end, F& f) {
    if (graph.is_directed()) {
      throw error("Shortest path works only on undirected graphs");
    }
    const auto n_vertices = graph.size();
    std::priority_queue<std::pair<weight_t, vertex_t>> pq;
    std::vector<weight_t> distances(n_vertices, D_INFINITY);
    std::vector<vertex_t> predecessors(n_vertices, NO_PREDECESSOR);
    std::vector<bool> visited(n_vertices, false);

    auto reconstruct_path = [&] () {
      std::vector<vertex_t> path;
      if (distances[end] == D_INFINITY) {
        return path;
      }
      auto current = end;
      do {
        path.push_back(current);
        current = predecessors[current];
      }
      while (current != NO_PREDECESSOR);
      std::ranges::reverse(path);
      return path;
    };

    distances[start] = 0;
    pq.push({f(start, end), start});
    while (!pq.empty()) {
      auto [_, v] = pq.top();
      auto d = distances[v];
      pq.pop();
      if (v == end) {
        return reconstruct_path();
      }
      visited[v] = true;
      for (const auto& edge : graph.get_neighbors(v)) {
        if (edge.weight <= 0) {
          throw error("All shortest paths: negative weights are not allowed");
        }
        if (visited[edge.end]) {
          continue;
        }
        auto new_d = d + edge.weight;
        if (new_d < distances[edge.end]) {
          distances[edge.end] = new_d;
          predecessors[edge.end] = v;
          pq.push({f(edge.end, end), edge.end});
        }
      }
    }
    return {};
  }

  template <GraphImpl Impl, VertexKey Vertex, PathCostFunction<Vertex> F>
  generator<Vertex> shortest_path(const Graph<Impl, Vertex, false>& graph, const Vertex& start, const Vertex& end, F& f) {
    auto cost_function = [&] (vertex_t u, vertex_t v) {
      return f(graph.get_vertex_from_internal_index(u), graph.get_vertex_from_internal_index(v));
    };
    auto path = shortest_path(graph.impl(), graph.get_internal_index(start), graph.get_internal_index(end), cost_function);
    for (const auto vertex: path) {
      co_yield graph.get_vertex_from_internal_index(vertex);
    }
  }

  template <GraphImpl Impl, VertexKey Vertex, PathCostFunction<Vertex> F>
  generator<Vertex> shortest_path(const Graph<Impl, Vertex, true>& graph, const Vertex& start, const Vertex& end, F& f) {
    static_assert(false, "Shortest paths works only on undirected graphs");
  }

}  // namespace grafology
