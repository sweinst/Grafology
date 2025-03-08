#include <grafology/graph.h>
#include <grafology/algorithms/topological_sort.h>
#include <print>

// Example: topological sort of a Graph

namespace g = grafology;
// a directed sparse graph which uses strings as vertices identifiers
using Graph = g::DirectedSparseGraph<std::string, int>;
using Edge = Graph::Edge;

std::vector<Edge> edges {
        {.start="Mix ingredients", .end="Add batter to pan"}, 
        {"Grease/Flour pan", "Add batter to pan"}, 
        {"Add batter to pan", "Bake cake"}, 
        {"Preheat oven", "Bake cake"}, 
        {"Bake cake", "Cool cake"}, 
        {"Cool cake", "Frost cake"}, 
        {"Make frosting", "Frost cake"}, 
        {"Frost cake", "Eat cake :-)"}, 
    };


int main () {
    int capacity = 10;
    Graph graph(capacity);
    graph.set_edges(edges, true);
    for (const auto& [group, vertex] : g::topological_sort(graph)) {
        std::println( "Group {}: '{}'", group, vertex);
    }
    return 0;
}
