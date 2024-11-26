#include <grafology/grafology.h>
#include <print>
using namespace std::string_literals;

// Example: depth first search of a Graph

namespace g = grafology;
// a directed sparse graph which uses strings as vertices identifiers
using Graph = g::DirectedSparseGraph<std::string>;
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
    std::string start{"Mix ingredients"};
    for (const auto& vertex : g::depth_first_search(graph, start)) {
        std::println( "{}", vertex);
    }
    return 0;
}
