#pragma once
#include "graph_impl.h"
#include <vector>
#include <algorithm>

namespace grafology {

    /** @brief A very simple flat map for mapping vertex destination index to weight
     * @details This is simple a sorted vector .
     */
    class FlatIndexMap {
        // TODO: batch initialization
        // TODO: batch insertion (using Tim or Smooth Sort (as data are almost sorted) ?)
        public: 
            FlatIndexMap() = default;
            FlatIndexMap(const FlatIndexMap&) = default;
            FlatIndexMap(FlatIndexMap&&) = default;
            FlatIndexMap& operator=(const FlatIndexMap&) = default;
            FlatIndexMap& operator=(FlatIndexMap&&) = default;
            ~FlatIndexMap() = default;

            /**
             * @brief A map element
             * @details
             * - the key is the edge end vertex index
             * - the value is the edge weight
             */
            struct MapEntry {
                vertex_t vertex;
                weight_t weight;
                
                auto operator<=>(const MapEntry& other) const {
                    return vertex <=> other.vertex;
                }
            };

            void set(vertex_t vertex, weight_t weight) {
                MapEntry key {vertex, weight};
                auto it = std::lower_bound(_flat_map.begin(), _flat_map.end(), key);
                if (it != _flat_map.end() && it->vertex == vertex) {
                    it->weight = weight;
                } else {
                    _flat_map.insert(it, std::move(key));
                }
            }

            void set(const MapEntry& entry) {
                set(entry.vertex, entry.weight);
            }

            weight_t get(vertex_t vertex) const {
                auto it = std::lower_bound(_flat_map.begin(), _flat_map.end(), MapEntry{vertex, 0});
                if (it != _flat_map.end() && it->vertex == vertex) {
                    return it->weight;
                }
                return 0;
            }

            void remove(vertex_t vertex) {
                auto it = std::lower_bound(_flat_map.begin(), _flat_map.end(), MapEntry{vertex, 0});
                if (it != _flat_map.end() && it->vertex == vertex) {
                    _flat_map.erase(it);
                }
            }

            // should be cbegin but the range-based loops won't work if defined as cbegin
            auto begin() const {
                return _flat_map.cbegin();
            }

            // should be cend but the range-based loops won't work if defined as cend
            auto end() const {
                return _flat_map.cend();
            }

            auto size() const {
                return _flat_map.size();
            }
        private:
            std::vector<MapEntry> _flat_map;
    };

} // namespace grafology
