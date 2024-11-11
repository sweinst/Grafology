#pragma once
#include <concepts>
#include <string>

namespace grafology {

//** the internal node type
using node_t = int;

//** the edge type */
struct edge_t {
    node_t start;
    node_t end;
    int weight;
};

/**
* @brief the requirements for a graph implementation
* 
* In an implementation:
* - node are represented by integers. These integers are private to the implementation.
* - edges are represented by (node start, node end, weight). For bidrected graph, each edge is represented by two edges.
*
* The requirements are:
* - must have a Node type which match the requirements of the concept Node
* - must be able to add a node
* - must be able to add an edge
* - must be able to remove a node
* - must be able to remove an edge
*/
template<typename G>
concept GraphImpl = requires(G g) {
    // { G::Node } -> Node;
   { g.label() }  -> std::convertible_to<std::string>;
    // { g.add_node(node_t) } -> std::convertible_to<bool>;
    // { g.add_edge(Impl::Node{}, Impl::Node{}) } -> std::convertible_to<bool>;
    // { g.remove_node(Impl::Node{}) } -> std::convertible_to<bool>;
    // { g.remove_edge(Impl::Node{}, Impl::Node{}) } -> std::convertible_to<bool>;
};

} // namespace grafology
