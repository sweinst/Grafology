#include "graph_impl.h"
#include <vector>
#include <cassert>

namespace grafology {
    /**
     * @brief A disjoint set data structure
     * @details This data structure is used to keep track of the connected components of a graph.
     * @remarks The implementation uses the path compression optimization so find is not const.
     */
    class DisjointSet {
        static constexpr vertex_t NPOS {std::numeric_limits<vertex_t>::max()};
        public:
            DisjointSet(size_t size) 
            : _parent(size, NPOS) {}

            vertex_t find(vertex_t v) {
                assert(v < _parent.size());
                if (_parent[v] == NPOS)
                {
                    return v;
                }

                // with path compression optimization
                auto r = find(_parent[v]);
                _parent[v] = r;
                return r;
            }

            bool merge(vertex_t v1, vertex_t v2) {
                assert(v1 < _parent.size() && v2 < _parent.size());
                vertex_t root1 = find(v1);
                vertex_t root2 = find(v2);
                if (root1 != root2) {
                    _parent[root1] = root2;
                    return true;
                }
                return false;
            }

            generator<vertex_t> roots() const {
                for (vertex_t i = 0; i < _parent.size(); ++i) {
                    if (_parent[i] == NPOS) {
                        co_yield i;
                    }
                }
            }
            
        private:
            std::vector<vertex_t> _parent;
    };
} // namespace grafology
