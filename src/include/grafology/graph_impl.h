#pragma once
#include <concepts>
#include <ranges>
#include <string>
#if defined(_MSC_VER)
    // don't know when generator will be available in MSVC ?
    #include <experimental/generator>
    template <class _Ty, class _Alloc = std::allocator<char>>
    using generator = std::experimental::generator<_Ty, _Alloc>;
#else
    #include <generator>
    template<typename _Ref, typename _Val = void, typename _Alloc = void>
    using generator = std::generator<_Ref, _Val, _Alloc>;
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

/** @brief The requirements for an iterator handling node_t 
 * @details The requirements are:
 * - must be an input iterator
 * - must be able to dereference the iterator to an node_t
 */
template <typename I>
concept input_iterator_vertex = 
    std::input_iterator<I> &&
    std::convertible_to<std::iter_value_t<I>, node_t>
;

/** @brief The requirements for a range handling node_t 
 * @details The requirements are:
 * - must be an input range
 * - must be able to dereference the iterator to an node_t
 */
template <typename R>
concept input_range_vertex = 
    std::ranges::input_range<R> &&
    std::convertible_to<std::iter_value_t<R>, node_t>
;

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

/** @brief The requirements for a range handling edge_t 
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
* - must be able to add one or more vertices
* - must be able to add one or more edges
* - must be able to remove a vertex
* @remarks It is not possible to set the constraints on templated functions without explicitly defining the template type.
* So they should be put in separated concepts and should be checked only in the instantiations or specializations.
* So I've left them aside.
*/
template<typename G>
concept GraphImpl = requires(G g, unsigned i, unsigned j, weight_t w) {
        // TODO: vertex removal
        {g.add_vertex()} -> std::convertible_to<node_t>;
        {g.add_vertices(i)} -> std::convertible_to<generator<node_t>>;
        {g.set_edge(i, j, w)};
        // g.set_edges(std::declval<input_iterator_edge>(), std::declval<std::sentinel_for<input_iterator_edge>>());
        // g.set_edges(std::declval<input_range_edge>());
    }
    && requires(const G g, unsigned i, unsigned j) {
        {g.operator()(i, j)} -> std::convertible_to<weight_t>;
    }
;

} // namespace grafology
