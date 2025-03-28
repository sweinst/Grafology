#include <grafology/flat_index_map.h>
#include <catch2/catch_test_macros.hpp>

namespace g = grafology;

using weight_t = int;
using FlatIndexMap = g::FlatIndexMap<int>;

TEST_CASE("Test FlatIndexMap", "[flatindexmap]")
{
    std::vector<FlatIndexMap::MapEntry> entries = {{2, 2}, {45, 45}, {33, 35}, {33, 33}, {3,3}};
    std::vector<FlatIndexMap::MapEntry> expected = {{2, 2}, {3,3}, {33, 33}, {45, 45}};
    FlatIndexMap map;
    for (const auto& entry : entries) {
        map.set(entry);
    }
    REQUIRE(map.size() == expected.size());
    for (const auto& [stored, expected] : std::views::zip(map, expected)) {
        REQUIRE(stored.vertex == expected.vertex);
        REQUIRE(stored.weight == expected.weight);
    }

    for (const auto& e: expected)
    {
        REQUIRE(map.get(e.vertex) == e.weight);
    }

    REQUIRE(map.get(1) == 0);
    REQUIRE(map.get(5) == 0);
    REQUIRE(map.get(100) == 0);
    
    map.set(1, 10);
    map.set(1, 1);
    map.set(5, 5);
    map.set(100, 10);
    map.set(100, 100);

    REQUIRE(map.size() == expected.size() + 3);

    REQUIRE(map.get(1) == 1);
    REQUIRE(map.get(5) == 5);
    REQUIRE(map.get(100) == 100);

    map.remove(1);
    map.remove(5);
    map.remove(100);

    REQUIRE(map.size() == expected.size());

    REQUIRE(map.get(1) == 0);
    REQUIRE(map.get(5) == 0);
    REQUIRE(map.get(100) == 0);
}


