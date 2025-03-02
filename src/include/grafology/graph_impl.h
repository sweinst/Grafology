#pragma once
#include <cassert>
#include <concepts>
#include <format>
#include <ranges>
#include <stdexcept>
#include <string>
#include "internal/generator.h"

namespace grafology {

  /** @brief a grafology exception
   * @details This exception allows the definition of errors specific to this library.
   */
  class error : public std::runtime_error {
   public:
    // inherit the constructors
    using std::runtime_error::runtime_error;
    // allow to use std::format directly in the constructor
    template <class... Args>
    error(std::format_string<Args...> fmt, Args&&... args)
        : std::runtime_error(std::format(fmt, std::forward<Args>(args)...)) {}
  };

  //** the internal vertex type
  using vertex_t = unsigned;
  constexpr vertex_t INVALID_VERTEX = std::numeric_limits<vertex_t>::max();
  constexpr vertex_t NO_PREDECESSOR = std::numeric_limits<vertex_t>::max();
  //** the type for the edge weights (can be overriden by defining the macro GRAFOLOGY_WEIGHT_TYPE) */
  #if !defined(GRAFOLOGY_WEIGHT_TYPE)
  #define GRAFOLOGY_WEIGHT_TYPE int
  #endif
  using weight_t = GRAFOLOGY_WEIGHT_TYPE;
  constexpr weight_t D_INFINITY = std::numeric_limits<weight_t>::max();
  //** a step on path  = (Vertex, distance from path) */
  using step_t = std::tuple<vertex_t, weight_t>;

  //** the edge type */
  struct edge_t {
    vertex_t start;
    vertex_t end = INVALID_VERTEX;
    weight_t weight = 1;

    bool operator==(const edge_t& other) const {
      return start == other.start && end == other.end;
    }

    auto operator<=>(const edge_t& other) const {
      auto res = start <=> other.start;
      if (res != std::strong_ordering::equal) {
        return res;
      }
      return end <=> other.end;
    }
  };

  struct EqualUndirectedEdges {
    bool operator()(const edge_t& a, const edge_t& b) const {
      return (a.start == b.start && a.end == b.end) || (a.start == b.end && a.end == b.start);
    }
  };
}  // namespace grafology

namespace std {
  template <>
  struct hash<grafology::edge_t> {
    std::size_t operator()(const grafology::edge_t& e) const {
      auto h1 = std::hash<grafology::vertex_t>{}(e.start);
      auto h2 = std::hash<grafology::vertex_t>{}(e.end);
      return (h1 ^ (h2 << 1));
    }
  };
}  // namespace std

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
  template <typename G>
  concept GraphImpl = requires(G g, vertex_t i, vertex_t j, weight_t w) {
    // TODO: vertex removal
    { g.add_vertex() } -> std::convertible_to<vertex_t>;
    { g.add_vertices(i) } -> std::convertible_to<generator<vertex_t>>;
    { g.set_edge(i, j, w) };
    // g.set_edges(std::declval<input_iterator_value<edge_t>>(),
    // std::declval<std::sentinel_for<input_iterator_edge>>());
    // g.set_edges(std::declval<input_range_value<edge_t>>());
  } && requires(const G g, vertex_t i, vertex_t j) {
    { g.is_directed() } -> std::convertible_to<bool>;
    { g.size() } -> std::convertible_to<std::size_t>;
    { g.capacity() } -> std::convertible_to<std::size_t>;
    { g.degree(i) } -> std::convertible_to<std::size_t>;
    { g.in_degree(i) } -> std::convertible_to<std::size_t>;
    // NB: VS2022 does not support the C++23 multidimensional subscript operator (i.e. a[i,j])
    { g.operator()(i, j) } -> std::convertible_to<weight_t>;
    { g.get_edge(i, j) } -> std::convertible_to<edge_t>;
    { g.has_edge(i, j) } -> std::convertible_to<bool>;
    { g.weight(i, j) } -> std::convertible_to<weight_t>;
    { g.get_raw_neighbors(i) } -> std::convertible_to<generator<vertex_t>>;
    { g.get_neighbors(i) } -> std::convertible_to<generator<edge_t>>;
    { g.get_raw_in_neighbors(i) } -> std::convertible_to<generator<vertex_t>>;
    { g.get_in_neighbors(i) } -> std::convertible_to<generator<edge_t>>;
    { g.get_all_edges() } -> std::convertible_to<generator<edge_t>>;
    { g.invert() } -> std::convertible_to<G>;
  } && requires(const G g1, const G g2) {
    { g1 == g2 } -> std::convertible_to<bool>;
  };
}  // namespace grafology

namespace std {
  /**
   * @brief Specialization of std::format for edge_t
   */
  template <>
  struct formatter<grafology::edge_t, char> {
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template <class FormatContext>
    auto format(const grafology::edge_t& v, FormatContext& ctx) const {
      return std::format_to(ctx.out(), "[start:{},end:{},weight:{}]", v.start, v.end, v.weight);
    }
  };

}  // namespace std
