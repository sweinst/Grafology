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
using node_t = unsigned;
//** the type for the edge weights
using weight_t = int;

//** the edge type */
struct edge_t {
    node_t start;
    node_t end;
    int weight;
};

/** @brief The requirements for an iterator handling values of type V 
 * @details The requirements are:
 * - must be an input iterator
 * - must be able to dereference the iterator to a V
 */
template <typename I, typename V>
concept input_iterator_value = 
    std::input_iterator<I> &&
    std::convertible_to<std::iter_value_t<I>, V>
;

/** @brief The requirements for a range handling values of type V
 * @details The requirements are:
 * - must be an input range
 * - must be able to dereference the iterator to a V
 */
template <typename R, typename V>
concept input_range_value = 
    std::ranges::input_range<R> &&
    std::convertible_to<std::iter_value_t<R>, V>
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
concept GraphImpl = requires(G g, node_t i, node_t j, weight_t w) {
        // TODO: vertex removal
        {g.add_vertex()} -> std::convertible_to<node_t>;
        {g.add_vertices(i)} -> std::convertible_to<generator<node_t>>;
        {g.set_edge(i, j, w)};
        // g.set_edges(std::declval<input_iterator_value<edge_t>>(), std::declval<std::sentinel_for<input_iterator_edge>>());
        // g.set_edges(std::declval<input_range_value<edge_t>>());
    }
    && requires(const G g, node_t i, node_t j) {
        {g.is_directed()} -> std::convertible_to<bool>;
        {g.size()} -> std::convertible_to<std::size_t>;
        {g.capacity()} -> std::convertible_to<std::size_t>;
        {g.degree(i)} -> std::convertible_to<std::size_t>;
        {g.in_degree(i)} -> std::convertible_to<std::size_t>;
        {g.operator()(i, j)} -> std::convertible_to<weight_t>;
        {g.get_raw_neighbors(i)} -> std::convertible_to<generator<node_t>>;
        {g.get_neighbors(i)} -> std::convertible_to<generator<edge_t>>; 
        {g.get_raw_in_neighbors(i)} -> std::convertible_to<generator<node_t>>;
        {g.get_in_neighbors(i)} -> std::convertible_to<generator<edge_t>>; 
        {g.invert()} -> std::convertible_to<G>;
    }
;

} // namespace grafology
