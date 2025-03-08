#pragma once

#include <limits>
#include <utility>
#include <concepts>

namespace grafology {
    /** @brief The requirements for a number type
     */
    template <typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

    
    /// @brief All the types and constants needed for a graph
    /// @tparam weight_t the type used for the edge weights
    template <Number weight_t = int>
    struct traits {
        using vertex_t = unsigned;
        struct edge_t {
            static constexpr vertex_t INVALID_VERTEX = std::numeric_limits<vertex_t>::max();
            static constexpr vertex_t NO_PREDECESSOR = std::numeric_limits<vertex_t>::max();
    
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

        static constexpr auto INVALID_VERTEX = edge_t::INVALID_VERTEX;
        static constexpr auto NO_PREDECESSOR = edge_t::NO_PREDECESSOR;
        static constexpr weight_t D_INFINITY = std::numeric_limits<weight_t>::max();
    };

    /*
      // TODO: re-introduce 

  struct EqualUndirectedEdges {
    bool operator()(const edge_t& a, const edge_t& b) const {
      return (a.start == b.start && a.end == b.end) || (a.start == b.end && a.end == b.start);
    }
  };
*/
}  // namespace grafology
