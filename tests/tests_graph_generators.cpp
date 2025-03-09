#include <grafology/generators/r3mat_generator.h>
#include <catch2/catch_template_test_macros.hpp>

namespace g = grafology;
using weight_t = int;

TEST_CASE("Test R3Mat", "[generators]") {
    // TODO: check the distribution of edges. It should be a power law with exponent -2
    // as explained in the paper, we should do a Kolmogorov–Smirnov statistic test
    // and check that the result is low enough

    for (auto directed : {true, false}) {
        for (auto size : {10, 100, 1'000, 10'000}) {
            {
                CAPTURE(size, directed, "sparse");
                auto gs = g::generate_r3mat_sparse_graph<weight_t>(size, size, directed, 0);
                REQUIRE(gs.size() == size);
            }
            {
                CAPTURE(size, directed, "dense");
                auto gs = g::generate_r3mat_dense_graph<weight_t>(size, size, directed, 0);
                REQUIRE(gs.size() == size);
            }
        }
    }
}
