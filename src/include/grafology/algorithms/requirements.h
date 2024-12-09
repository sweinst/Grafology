#pragma once
#include <functional>
#include "../graph.h"

namespace grafology {
  /**
   * @brief The requirement for having a "<" operator
   */
  template <typename T>
  concept HasLessThan = requires(T u, T v) {
    { u < v } -> std::convertible_to<bool>;
  };

  /**
   * @brief The requirements for the cost function used for shortest paths on GraphImpl
   * @details The cost function must have the signature "double (*f)(vertex_t start, vertex_t end)"
   * and must return a value that can be compared with the "<" operator
   * @tparam F The function type
   */
  template <typename F>
  concept PathCostFunctionImpl =
      std::invocable<F&, vertex_t, vertex_t> && requires(F& f, vertex_t u, vertex_t v) {
        { f(v, u) } -> HasLessThan;
      };

  /**
   * @brief The requirements for the cost function used for shortest paths on Graph
   * @details The cost function must have the signature "double (*f)(const VertexKey& start, const
   * VertexKey& end)" and must return a value that can be compared with the "<" operator
   * @tparam F The function type
   */
  template <typename F, typename V>
  concept PathCostFunction =
      VertexKey<V>
      && std::invocable<F&, const V&, const V&> 
      && requires(F& f, const V& u, const V& v) {
        { f(v, u) } -> HasLessThan;
      };

}  // namespace grafology