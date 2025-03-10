#pragma once
#include <cassert>
#include <ranges>
#include "utilities.h"
#include "graph_traits.h"

namespace grafology {
  /** @brief The requirements for an iterator handling values of type V
   * @details The requirements are:
   * - must be an input iterator
   * - must be able to dereference the iterator to a V
   */
  template <typename I, typename V>
  concept input_iterator_value =
      std::input_iterator<I> && std::convertible_to<std::iter_value_t<I>, V>;

  /** @brief The requirements for a range handling values of type V
   * @details The requirements are:
   * - must be an input range
   * - must be able to dereference the iterator to a V
   */
  template <typename R, typename V>
  concept input_range_value =
      std::ranges::input_range<R> && std::convertible_to<std::iter_value_t<R>, V>;

  /**
   * @brief the requirements for a graph implementation
   * @details
   * In an implementation:
   * - vertex are represented by integers. These integers are private to the implementation.
   * - edges are represented by (vertex start, vertex end, weight). For bidrected graph, each edge
   * is represented by two edges.
   *
   * The requirements are:
   * - must have a Vertex type which match the requirements of the concept VertexKey
   * - must be able to add one or more vertices
   * - must be able to add one or more edges
   * - must be able to remove a vertex
   * @remarks It is not possible to set the constraints on templated functions without explicitly
   * defining the template type. So they should be put in separated concepts and should be checked
   * only in the instantiations or specializations. So I've left them aside.
   */
  template <typename G, typename weight_t>
  concept GraphImpl = 
    Number<weight_t>
  && requires(G g, vertex_t i, vertex_t j, weight_t w) {
    // TODO: vertex removal
    { g.add_vertex() } -> std::convertible_to<vertex_t>;
    { g.add_vertices(i) } -> std::convertible_to<generator<vertex_t>>;
    { g.set_edge(i, j, w) };
    // g.set_edges(std::declval<input_iterator_value<edge_t<weight_t>>>(),
    // std::declval<std::sentinel_for<input_iterator_edge>>());
    // g.set_edges(std::declval<input_range_value<edge_t<weight_t>>>());
   } && requires(const G g, vertex_t i, vertex_t j) {
    { g.is_directed() } -> std::convertible_to<bool>;
    { g.size() } -> std::convertible_to<std::size_t>;
    { g.capacity() } -> std::convertible_to<std::size_t>;
    { g.degree(i) } -> std::convertible_to<std::size_t>;
    { g.in_degree(i) } -> std::convertible_to<std::size_t>;
    // NB: VS2022 does not support the C++23 multidimensional subscript operator (i.e. a[i,j])
    { g.operator()(i, j) } -> std::convertible_to<weight_t>;
    { g.get_edge(i, j) } -> std::convertible_to<edge_t<weight_t>>;
    { g.has_edge(i, j) } -> std::convertible_to<bool>;
    { g.weight(i, j) } -> std::convertible_to<weight_t>;
    { g.get_raw_neighbors(i) } -> std::convertible_to<generator<vertex_t>>;
    { g.get_neighbors(i) } -> std::convertible_to<generator<edge_t<weight_t>>>;
    { g.get_raw_in_neighbors(i) } -> std::convertible_to<generator<vertex_t>>;
    { g.get_in_neighbors(i) } -> std::convertible_to<generator<edge_t<weight_t>>>;
    { g.get_all_edges() } -> std::convertible_to<generator<edge_t<weight_t>>>;
    { g.invert() } -> std::convertible_to<G>;
  } && requires(const G g1, const G g2) {
    { g1 == g2 } -> std::convertible_to<bool>;
  };
}  // namespace grafology

namespace std {
  /**
   * @brief Specialization of std::format for edge_t
   */
  template <grafology::Number weight_t>
  struct formatter<grafology::edge_t<weight_t>, char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <class FormatContext>
    auto format(const grafology::edge_t<weight_t>& v, FormatContext& ctx) const {
      return std::format_to(ctx.out(), "[start:{},end:{},weight:{}]", v.start, v.end, v.weight);
    }
  };

}  // namespace std
