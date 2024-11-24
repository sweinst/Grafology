#include <grafology/grafology.h>
#include <print>

namespace g = grafology;
// a directed sparse graph which uses strings as vertices identifiers
using Graph = g::DirectedSparseGraph<std::string>;
using Edge = Graph::Edge;

std::vector<Edge> edges {
        {"Mix ingredients", "Add batter to pan"}, 
        {"Grease/Flour pan", "Add batter to pan"}, 
        {"Add batter to pan", "Bake cake"}, 
        {"Preheat oven", "Bake cake"}, 
        {"Bake cake", "Cool cake"}, 
        {"Cool cake", "Frost cake"}, 
        {"Make frosting", "Frost cake"}, 
        {"Frost cake", "Eat cake :-)"}, 
    };


int main () {
    Graph graph(10);
    graph.set_edges(edges, true);
    for (const auto& [group, vertex] : g::topological_sort(graph)) {
        std::println( "Group {}: '{}'", group, vertex);
    }
    return 0;
}
