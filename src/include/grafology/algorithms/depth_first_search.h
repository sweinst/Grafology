#pragma once
#include "../graph.h"
#include <stack>

namespace grafology {

    /**
     * @brief Perform a depth-first search on a GraphImpl
     * @param graph The graph to search
     * @package start The vertex to start the search from
     */
    template <GraphImpl G>
    generator<vertex_t> depth_first_search(const G& graph, vertex_t start) {
        std::vector<bool> visited(graph.size(), false);
        std::stack<vertex_t> stack;
        stack.push(start);
        while (!stack.empty()) {
            vertex_t current = stack.top();
            stack.pop();
            if (!visited[current]) {
                visited[current] = true;
                co_yield current;
                for (auto neighbour : graph.get_raw_neighbors(current)) {
                    stack.push(neighbour);
                }
            }
        }
    }

    /** 
     * @brief A class to repeatedly find a path between two vertices in a GraphImpl using depth-first search. 
     * All the containers used for the search are allocated only once.
     */
    template <GraphImpl G>
    class DFSPathFinder {
        public:
            DFSPathFinder(G& graph) 
            : _graph(graph), 
              _visited(graph.size(), false), 
              _parent(graph.size(), NO_PREDECESSOR),
              _start(NO_PREDECESSOR),
              _end(NO_PREDECESSOR) {
            }
            bool find(vertex_t start, vertex_t end) {
                std::stack<vertex_t> stack;
                _start = start;
                _end = end;
                _visited.assign(_graph.size(), false);
                _parent.assign(_graph.size(), NO_PREDECESSOR);

                stack.push(_start);
                while (!stack.empty()) {
                    vertex_t current = stack.top();
                    stack.pop();
                    if (!_visited[current]) {
                        _visited[current] = true;
                        for (auto neighbour : _graph.get_raw_neighbors(current)) {
                            stack.push(neighbour);
                            _parent[neighbour] = current;
                            if (neighbour == _end) {
                                return true;
                            }
                        }
                    }
                }
                return false;
            }

            generator<vertex_t> get_path() {
                std::vector<vertex_t> path;
                if (_start == NO_PREDECESSOR || _end == NO_PREDECESSOR) {
                    return path;
                }
                vertex_t neighbour = _end;
                while (neighbour != NO_PREDECESSOR) {
                    co_yield neighbour;
                    neighbour = _parent[neighbour];
                }
            }

            vertex_t get_parent(vertex_t vertex) {
                return _parent[vertex];
            }

        private:
            G& _graph;
            std::vector<bool> _visited;
            std::vector<vertex_t> _parent;
            vertex_t _start;
            vertex_t _end;
    };

    /**
     * @brief Perform a depth-first search on a Graph
     * @param graph The graph to search
     * @package start The vertex to start the search from
     */
    template<GraphImpl Impl, VertexKey Vertex, bool IsDirected>
    generator<Vertex> depth_first_search(const Graph<Impl, Vertex, IsDirected>& graph, const Vertex& start) {
        const auto impl = graph.impl();
        auto idx_start = graph.get_internal_index(start);
        if (idx_start == -1) {
            throw error("depth_first_search: Initial vertex '{}' not found", start);
        }
        for (auto vertex : depth_first_search(impl, idx_start)) {
            co_yield graph.get_vertex_from_internal_index(vertex);
        }
    }

} // namespace grafology
