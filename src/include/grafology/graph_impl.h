#pragma once
#include <concepts>
#include <ranges>
#include <string>

namespace grafology {

//** the internal node type
using node_t = int;
//** the type for the edge weights
using weight_t = int;

//** the edge type */
struct edge_t {
    node_t start;
    node_t end;
    int weight;
};

/** @brief The requirements for an iterator handling edge_t 
 * @details The requirements are:
 * - must be an input iterator
 * - must be able to dereference the iterator to an edge_t
 */
template <typename I>
concept input_iterator_edge = 
    std::input_iterator<I> &&
    std::convertible_to<std::iter_value_t<I>, edge_t>
;

/** @brief The requirements for an iterator handling edge_t 
 * @details The requirements are:
 * - must be an input range
 * - must be able to dereference the iterator to an edge_t
 */
template <typename R>
concept input_range_edge = 
    std::ranges::input_range<R> &&
    std::convertible_to<std::iter_value_t<R>, edge_t>
;

/**
* @brief the requirements for a graph implementation
* @details
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
