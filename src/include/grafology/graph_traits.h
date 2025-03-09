#pragma once

#include <unordered_map>
#include <concepts>
#include <limits>
#include <utility>

namespace grafology {
    /** @brief The requirements for a number type
     */
    template <typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

    //** the internal vertex type
    using vertex_t = unsigned;
    static constexpr vertex_t INVALID_VERTEX = std::numeric_limits<vertex_t>::max();
    static constexpr vertex_t NO_PREDECESSOR = std::numeric_limits<vertex_t>::max();

    //** the internal edge type
    template <Number weight_t>
    struct edge_t {

        vertex_t start;
        vertex_t end = INVALID_VERTEX;
        weight_t weight = 1;
        
        static constexpr weight_t D_INFINITY = std::numeric_limits<weight_t>::max();

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

        std::size_t hash() const {
            auto h1 = std::hash<vertex_t>{}(start);
            auto h2 = std::hash<vertex_t>{}(end);
            return (h1 ^ (h2 << 1));
        }
    };

    template <Number weight_t>
    using step_t = std::tuple<vertex_t, weight_t>;

    template <Number weight_t>
    struct EqualUndirectedEdges {
        bool operator()(const edge_t<weight_t>a, const edge_t<weight_t>& b) const {
          return (a.start == b.start && a.end == b.end) || (a.start == b.end && a.end == b.start);
        }
    };

}  // namespace grafology

namespace std {
    template <grafology::Number weight_t>
    struct hash<grafology::edge_t<weight_t>> {
        std::size_t operator()(const grafology::edge_t<weight_t>& e) const {
            return e.hash();
        }
    };
}  // namespace std
