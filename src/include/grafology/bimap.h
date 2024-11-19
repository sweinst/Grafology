#pragma once
#include "vertex.h"
#include <unordered_map>

namespace grafology
{
    /**
     * @brief A crude bidirectional map between Vertex and internal indexes.
     */
    template <Vertex V>
    class BiMap {
        // TODO: check bounds in debug builds
    public:
        BiMap(unsigned capacity) :
            _capacity(capacity), _size(0) {
            _index_to_vertex.reserve(capacity);
        }

        unsigned size() const {
            return _size;
        }

        unsigned capacity() const {
            return _capacity;
        }

        const V& get_vertex(unsigned i) const {
            return _index_to_vertex[i];
        }

        unsigned get_index(const V& v) const {
            return _vertex_to_index.find(v)->second;
        }

        // "pass by value then move" idiom
        unsigned add_vertex(V v) {
            auto it = _vertex_to_index.find(v);
            if (it != _vertex_to_index.end()) {
                return it->second;
            }
            _vertex_to_index[v] = _size;
            _index_to_vertex.push_back(std::move(v));
            return _size++;
        }

    private:
        std::vector<V> _index_to_vertex;
        std::unordered_map<V, unsigned> _vertex_to_index;
        const unsigned _capacity;
        unsigned _size;

    };
} // namespace grafology
