#include <grafology/graph.h>
#include <grafology/algorithms/topological_sort.h>
#include <print>

// Example: topological sort of a GraphImpl


namespace g = grafology;
// a directed sparse graph which uses strings as vertices identifiers
using Graph = g::SparseGraphImpl<int>;
using Edge = Graph::edge_lt;

// this is the same graph than in ex01-topological-sort-graph.cpp (cake recipe)
std::vector<Edge> edges {
        {.start=0, .end=1}, 
        {2, 1}, 
        {1, 3}, 
        {4, 3}, 
        {3, 5}, 
        {5, 6}, 
        {7, 6}, 
        {6, 8}, 
    };


int main () {
    int capacity = 10;
    int n_vertices = 9;
    
    Graph graph(capacity, n_vertices, true);
    graph.set_edges(edges);
    for (const auto& [group, vertex] : g::topological_sort(graph)) {
        std::println( "Group {}: {}", group, vertex);
    }
    return 0;
}
