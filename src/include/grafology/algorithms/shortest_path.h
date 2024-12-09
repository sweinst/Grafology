#pragma once
#include <functional>
#include <queue>
#include "../graph.h"

/**
 * @brief The requirement for the type T: it must implement the "<" operator
 */
namespace grafology {
  template <typename T>
  concept HasLessThan = requires(T u, T v) {
    { u < v } -> std::convertible_to<bool>;
  };

  /**
   * @brief The requirements for the cost function used for shortest paths on GraphImpl
   * @details The cost function must have the signature "double (*f)(vertex_t start, vertex_t end)"
   * and must return a value that can be compared with the "<" operator
   * @tparam F The function type
   */
  template <typename F>
  concept PathCostFunctionImpl =
      std::invocable<F&, vertex_t, vertex_t> && requires(F& f, vertex_t u, vertex_t v) {
        { f(v, u) } -> HasLessThan;
      };

  /**
   * @brief Compute the shortest path from one vertex to another
   * @remark this is based on the A* algorithm
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

}  // namespace grafology
