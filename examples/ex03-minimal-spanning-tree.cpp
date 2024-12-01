#include <grafology/algorithms/minimum_spanning_tree.h>
#include <print>

// Example: minimal spanning tree
// minimal circuit for visiting all Venice's main sites
// copied from https://www.hackerearth.com/blog/developers/kruskals-minimum-spanning-tree-algorithm-example

namespace g = grafology;

// a directed sparse graph which uses strings as vertices identifiers
struct Vertex {
    std::string _id;
    std::string _label {};
    bool operator==(const Vertex& other) const noexcept {
        return _id == other._id;
    }
};

namespace std {
    // we need to be able to put in a hash map
    template <>
    struct hash<Vertex> {
        std::size_t operator() (const Vertex& v) const {
            return std::hash<std::string>{}(v._id);
        }
    };
    // we need to be able to print it
    template <>
    struct formatter<Vertex, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            return ctx.begin();
        }

        template <class FormatContext>
        auto format(const Vertex& v, FormatContext& ctx) const {
            return std::format_to(ctx.out(), "{} ({})", v._label, v. _id);
        }
    };
}

// check that all requirements are met
static_assert(g::VertexKey<Vertex>);


using Graph = g::UndirectedSparseGraph<Vertex>;
using Edge = Graph::Edge;

std::vector <Vertex> vertices {
    {"A", "Cannaregio"},
    {"B", "Ponte Scalzi"},
    {"C", "Santa Corce"},
    {"D", "Dell ‘Orto"},
    {"E", "Ferrovia"},
    {"F", "Piazzale Roma"},
    {"G", "San Polo"},
    {"H", "Dorso Duro"},
    {"I", "San Marco"},
    {"J", "St. Mark Basilica"},
    {"K", "Castello"},
    {"L", "Arsenale"},
};

std::vector<Edge> edges {
    {{"B"}, {"C"}, 1},
    {{"I"}, {"J"}, 1},
    {{"B"}, {"E"}, 2},
    {{"C"}, {"G"}, 2},
    {{"G"}, {"I"}, 2},
    {{"C"}, {"D"}, 2},
    {{"K"}, {"L"}, 3},
    {{"E"}, {"F"}, 4},
    {{"A"}, {"B"}, 6},
    {{"A"}, {"C"}, 6},
    {{"A"}, {"D"}, 6},
    {{"E"}, {"C"}, 7},
    {{"J"}, {"L"}, 8},
    {{"F"}, {"H"}, 10},
    {{"F"}, {"G"}, 11},
    {{"H"}, {"I"}, 12},
    {{"I"}, {"K"}, 16},
    {{"D"}, {"J"}, 18},
    {{"G"}, {"H"}, 22},
    {{"H"}, {"K"}, 25}, 
};

int main() {
    Graph graph(vertices.size());
    graph.add_vertices(vertices);
    graph.set_edges(edges);

    auto mst = g::minimum_spanning_tree(graph);

    for (auto e: mst.get_all_edges())
    {
        std::println("{}", e);
    }
    

    return 0;
}