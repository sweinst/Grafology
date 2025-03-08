#include <limits>
#include <utility>
#include <concepts>

namespace grafology {
    template <typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

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
}  // namespace grafology
