#include <grafology/algorithms/breath_first_search.h>
#include <grafology/algorithms/depth_first_search.h>
#include "utils.h"

// Example: depth first search of a Graph

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
    
    Graph graph(capacity, n_vertices, false);
    graph.set_edges(edges);


    print_frame("DFS");
    // works with VS 17.12 but not with g++ 14.2
    //std::println( "{}", g::depth_first_search(graph, 0));
    std::print("[");
    for (const auto vertex : g::depth_first_search(graph, 0)) {
        std::print( "{}, ", vertex);
    }
    std::println("]");
    
    print_frame("BFS");
    // works with VS 17.12 but not with g++ 14.2
    //std::println( "{}", g::breath_first_search(graph, 0));
    std::print("[");
    for (const auto vertex : g::breath_first_search(graph, 0)) {
        std::println( "{}, ", vertex);
    }
    std::println("]");
    return 0;
}
