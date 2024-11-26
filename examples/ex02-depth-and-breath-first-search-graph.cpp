#include <grafology/algorithms/breath_first_search.h>
#include <grafology/algorithms/depth_first_search.h>
#include <print>
using namespace std::string_literals;

// Example: depth and breath first search of a Graph

namespace g = grafology;
// a directed sparse graph which uses strings as vertices identifiers
using Graph = g::UndirectedSparseGraph<std::string>;
using Edge = Graph::Edge;

namespace 
{
    void print_frame(const std::string_view title) {
        std::println("{:=^20}", "");
        std::println("|{: ^18}|", title);
        std::println("{:=^20}", "");
    }
    
} // namespace 


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

    print_frame("DFS");
    std::println( "{}", g::depth_first_search(graph, start));
    // i.e.:
    // for (const auto& vertex : g::depth_first_search(graph, start)) {
    //     std::println( "{}", vertex);
    // }

    print_frame("BFS");
    std::println( "{}", g::breath_first_search(graph, start));
    // i.e.:
    // for (const auto& vertex : g::breath_first_search(graph, start)) {
    //     std::println( "{}", vertex);
    // }
    return 0;
}
