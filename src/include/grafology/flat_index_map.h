#pragma once
#include "graph_impl.h"
#include <vector>
#include <algorithm>

namespace grafology {

    /** @brief A very simple flat map for mapping node destination index to weight
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
             * - the key is the edge end node index
             * - the value is the edge weight
             */
            struct MapEntry {
                node_t node;
                weight_t weight;
                
                auto operator<=>(const MapEntry& other) const {
                    return node <=> other.node;
                }
            };

            void set(node_t node, weight_t weight) {
                MapEntry key {node, weight};
                auto it = std::lower_bound(_flat_map.begin(), _flat_map.end(), key);
                if (it != _flat_map.end() && it->node == node) {
                    it->weight = weight;
                } else {
                    _flat_map.insert(it, std::move(key));
                }
            }

            void set(const MapEntry& entry) {
                set(entry.node, entry.weight);
            }

            weight_t get(node_t node) const {
                auto it = std::lower_bound(_flat_map.begin(), _flat_map.end(), MapEntry{node, 0});
                if (it != _flat_map.end() && it->node == node) {
                    return it->weight;
                }
                return 0;
            }

            void remove(node_t node) {
                auto it = std::lower_bound(_flat_map.begin(), _flat_map.end(), MapEntry{node, 0});
                if (it != _flat_map.end() && it->node == node) {
                    _flat_map.erase(it);
                }
            }

            auto begin() const {
                return _flat_map.begin();
            }

            auto end() const {
                return _flat_map.end();
            }

            auto size() const {
                return _flat_map.size();
            }
        private:
            std::vector<MapEntry> _flat_map;
    };

} // namespace grafology
