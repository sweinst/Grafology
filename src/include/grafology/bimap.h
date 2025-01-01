#pragma once
#include "vertex.h"
#include <unordered_map>
#include <cassert>

namespace grafology
{
    /**
     * @brief A crude bidirectional map between Vertex and internal indexes.
     */
    template <VertexKey V>
    class BiMap {
        // TODO: check bounds in debug builds
    public:
        BiMap(unsigned capacity) :
            _capacity(capacity), _size(0) {
            _index_to_vertex.reserve(capacity);
        }

        BiMap(const BiMap&) = default;
        BiMap(BiMap&&) = default;
        BiMap& operator=(const BiMap&) = default;
        BiMap& operator=(BiMap&&) = default;
        bool operator==(const BiMap& other) const = default;
        bool operator!=(const BiMap& other) const = default;

        unsigned size() const {
            return _size;
        }

        unsigned capacity() const {
            return _capacity;
        }

        const V& get_vertex(unsigned i) const {
            assert(i < _size);
            return _index_to_vertex[i];
        }

        unsigned get_index(const V& v) const {
            auto i = _vertex_to_index.find(v);
            if (i == _vertex_to_index.end())
            {
                return INVALID_VERTEX;
            }
            return i->second;
        }

        // "pass by value then move" idiom
        unsigned add_vertex(V v) {
            auto it = _vertex_to_index.find(v);
            if (it != _vertex_to_index.end()) {
                return it->second;
            }
            assert(_size < _capacity);
            _vertex_to_index[v] = _size;
            _index_to_vertex.push_back(std::move(v));
            return _size++;
        }

    private:
        std::vector<V> _index_to_vertex;
        std::unordered_map<V, unsigned> _vertex_to_index;
        unsigned _capacity;
        unsigned _size;

    };
} // namespace grafology
