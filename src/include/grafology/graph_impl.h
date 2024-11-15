#pragma once
#include <concepts>
#include <ranges>
#include <string>
#if defined(_MSC_VER)
    // don't know when generator will be available in MSVC ?
    #include <experimental/generator>
    namespace experimental = std;
#else
    #include <generator>
#endif 

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
* @remarks It is not possible to set the constraints on templated functions without explicitly defining the template type.
* So they should be put in separated concepts and should be checked only in the instantiations or specializations.
* So I've left them aside.
*/
template<typename G>
concept GraphImpl = requires(G g, unsigned i, unsigned j, weight_t w) {
    // { G::Node } -> Node;
    {g.operator()(i, j)} -> std::convertible_to<weight_t&>;
    {g.set_edge(i, j, w)};
    // g.set_edges(std::declval<input_iterator_edge>(), std::declval<std::sentinel_for<input_iterator_edge>>());
    // g.set_edges(std::declval<input_range_edge>());
    // { g.add_node(node_t) } -> std::convertible_to<bool>;
    // { g.add_edge(Impl::Node{}, Impl::Node{}) } -> std::convertible_to<bool>;
    // { g.remove_node(Impl::Node{}) } -> std::convertible_to<bool>;
    // { g.remove_edge(Impl::Node{}, Impl::Node{}) } -> std::convertible_to<bool>;
    }
    && requires(const G g, unsigned i, unsigned j) {
    {g.operator()(i, j)} -> std::convertible_to<weight_t>;
    }
;

// } // namespace grafology
// concept GraphImpl = requires(G g, unsigned i, unsigned j, weight_t w) {

} // namespace grafology
