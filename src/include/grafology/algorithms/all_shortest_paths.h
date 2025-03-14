#pragma once
#include <queue>
#include "../graph.h"

namespace grafology {
    //==============================================================================
    // Implementation Graphs
    //==============================================================================

    /**
     * @brief This struct allows to process the results of the algorithm all_shortest_paths
     * @remark it allows to save the results of the algorithm and so avoid to use dangling
     * references
     */
    template <typename weight_t>
    struct AllShortestPathsImpl {
        static constexpr auto D_INFINITY = edge_t<weight_t>::D_INFINITY;
        using step_lt = std::tuple<vertex_t, weight_t>;

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

        std::vector<step_lt> get_path(vertex_t end) const {
            assert(end < _distances.size());
            std::vector<step_lt> path;
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

        std::vector<vertex_t> get_raw_path(vertex_t end) const {
            assert(end < _distances.size());
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
            std::ranges::reverse(path);
            return path;
        }

        void reset(vertex_t start) {
            _start = start;
            std::fill(_distances.begin(), _distances.end(), D_INFINITY);
            std::fill(_predecessors.begin(), _predecessors.end(), NO_PREDECESSOR);
        }
    };

    /**
     * @brief Compute the shortest paths from a vertex to all other vertices in a graph
     * @remark this is based on the Dijkstra's algorithm
     */
    template <typename G>
        requires GraphImpl<G, typename G::weight_lt>
    AllShortestPathsImpl<typename G::weight_lt> all_shortest_paths(const G& graph, vertex_t start) {
        using weight_lt = typename G::weight_lt;

        assert(start < graph.size());

        const auto n_vertices = graph.size();
        AllShortestPathsImpl<weight_lt> res(n_vertices, start);
        std::priority_queue<std::pair<weight_lt, vertex_t>> pq;

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
                // Dijkstra's algorithm doesn't work with negative weights
                assert(edge.weight > 0);
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
     * @brief Compute the shortest paths from a vertex to all other vertices in a graph
     * @remark this is based on the Bellman-Ford algorithm
     */
    template <typename G>
        requires GraphImpl<G, typename G::weight_lt>
    AllShortestPathsImpl<typename G::weight_lt>
    all_shortest_paths_BF(const G& graph, vertex_t start) {
      assert(start < graph.size());

      if (!graph.is_directed()) {
          throw std::runtime_error("Bellman-Ford algorithm only works with directed graphs");
      }

      using weight_lt = typename G::weight_lt;
      static constexpr auto D_INFINITY = edge_t<weight_lt>::D_INFINITY;

      const auto n_vertices = graph.size();
      AllShortestPathsImpl<weight_lt> res(n_vertices, start);
      res._distances[start] = 0;
      for (auto idx = 0; idx < n_vertices; ++idx) {
          for (const auto edge : graph.get_all_edges()) {
              if (res._distances[edge.start] != D_INFINITY) {
                  auto new_d = res._distances[edge.start] + edge.weight;
                  if (new_d < res._distances[edge.end]) {
                      if (idx == n_vertices - 1) {
                          throw std::runtime_error("Negative cycle detected");
                      }
                      res._distances[edge.end] = new_d;
                      res._predecessors[edge.end] = edge.start;
                  }
              }
          }
      }
      return res;
    }

    //==============================================================================
    // Graphs
    //==============================================================================

    /**
     * @brief Compute the shortest paths from a vertex to all other vertices in a
     * graph
     * @remark this is based on the Dijkstra's algorithm
     */
    template <typename Impl, VertexKey Vertex, bool IsDirected>
        requires GraphImpl<Impl, typename Impl::weight_lt>
    struct AllShortestPaths {
        using weight_lt = typename Impl::weight_lt;
        using step_lt = Step<Vertex, weight_lt>;
        static constexpr auto D_INFINITY = edge_t<weight_lt>::D_INFINITY;

        AllShortestPaths(
            AllShortestPathsImpl<weight_lt>&& shortest_paths,
            const Graph<Impl, Vertex, IsDirected, typename Impl::weight_lt>& graph
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

        weight_lt get_distance(const Vertex& v) const {
            assert(graph.get_internal_index(v) != INVALID_VERTEX);
            return _shortest_paths._distances[graph.get_internal_index(v)];
        }

        const Vertex& get_predecessor(const Vertex& v) const {
            assert(graph.get_internal_index(v) != INVALID_VERTEX);
            return graph.get_vertex_from_internal_index(
                _shortest_paths._predecessors[graph.get_internal_index(v)]
            );
        }

        generator<step_lt> get_path(const Vertex& v) const {
            assert(graph.get_internal_index(v) != INVALID_VERTEX);
            auto sp = _shortest_paths.get_path(graph.get_internal_index(v));
            for (const auto& [v, d] : sp) {
                co_yield std::make_tuple(graph.get_vertex_from_internal_index(v), d);
            }
        }

       private:
        const AllShortestPathsImpl<weight_lt> _shortest_paths;
        const Graph<Impl, Vertex, IsDirected, weight_lt>& graph;
    };

    template <typename Impl, VertexKey Vertex, bool directed>
        requires GraphImpl<Impl, typename Impl::weight_lt>
    AllShortestPaths<Impl, Vertex, directed> all_shortest_paths(
        const Graph<Impl, Vertex, directed, typename Impl::weight_lt>& graph,
        const Vertex& start
    ) {
        assert(graph.get_internal_index(start) != INVALID_VERTEX);
        auto sp_impl = all_shortest_paths(graph.impl(), graph.get_internal_index(start));
        return AllShortestPaths<Impl, Vertex, directed>(std::move(sp_impl), graph);
    }

    template <typename Impl, VertexKey Vertex>
        requires GraphImpl<Impl, typename Impl::weight_lt>
    AllShortestPaths<Impl, Vertex, true> all_shortest_path_BF(
        const Graph<Impl, Vertex, true, typename Impl::weight_lt>& graph,
        const Vertex& start
    ) {
        assert(graph.get_internal_index(start) != INVALID_VERTEX);
        auto sp_impl = all_shortest_paths(graph.impl(), graph.get_internal_index(start));
        return AllShortestPaths<Impl, Vertex, true>(std::move(sp_impl), graph);
    }

    template <typename Impl, VertexKey Vertex>
        requires GraphImpl<Impl, typename Impl::weight_lt>
    AllShortestPaths<Impl, Vertex, false> all_shortest_path_BF(
        const Graph<Impl, Vertex, false, typename Impl::weight_lt>& graph,
        const Vertex& start
    ) {
      static_assert(false, "Bellman-Ford algorithm only works with directed graphs");
    }

  }  // namespace grafology
