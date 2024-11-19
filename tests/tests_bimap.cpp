#include "test_vertex.h"
#include <grafology/bimap.h>
#include <catch2/catch_test_macros.hpp>

namespace g = grafology;

TEST_CASE("Test BiMap", "[bimap]") {
    using BiMap = g::BiMap<TestVertex>;
    std::vector<TestVertex> init = {
      {1, "one"},
      {2, "two"},
      {3, "three"},
      {4, "four"},
      {5, "five"},
      {6, "six"},
      {7, "seven"},
    };

    BiMap b(10);
    for (const auto& v: init) {
        auto idx = b.add_vertex(v);
        CHECK(idx + 1 == v._id);
    }

    for (const auto& v: init) {
        CAPTURE(v._id, v._label);
        CHECK(b.get_index(v) + 1 == v._id);
        CHECK(b.get_vertex(b.get_index(v)) == v);
    }

}