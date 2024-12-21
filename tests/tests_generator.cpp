#include <catch2/catch_template_test_macros.hpp>
#include <grafology/internal/generator_impl.h>

std::generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i) {
        co_yield i;
    }
}

TEST_CASE("Test Generator", "[internals]") {
    for (auto i : range(0, 10)) {
        REQUIRE(i >= 0);
    }    
}
