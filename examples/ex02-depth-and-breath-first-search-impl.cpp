#include <grafology/algorithms/breath_first_search.h>
#include <grafology/algorithms/depth_first_search.h>
#include <print>

// Example: depth first search of a Graph

namespace g = grafology;
// a directed sparse graph which uses strings as vertices identifiers
using Graph = g::SparseGraphImpl;
using Edge = g::edge_t;

namespace 
{
    void print_frame(const std::string_view title) {
        std::println("{:=^20}", "");
        std::println("|{: ^18}|", title);
        std::println("{:=^20}", "");
    }
    
} // namespace 

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
    std::println( "{}", g::depth_first_search(graph, 0));
    // i.e.:
    // for (const auto vertex : g::depth_first_search(graph, 0)) {
    //     std::println( "{}", vertex);
    // }
    print_frame("BFS");
    std::println( "{}", g::breath_first_search(graph, 0));
    // i.e.:
    // for (const auto vertex : g::breath_first_search(graph, 0)) {
    //     std::println( "{}", vertex);
    // }
    return 0;
}
