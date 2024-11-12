#include <catch2/catch_test_macros.hpp>
#include <grafology/grafology.h>

#include <grafology/grafology.h>
//#include <fmt/core.h>

namespace g = grafology;

namespace{
    struct TestVertex{

        int _id;
        std::string _label;

        bool operator == (const TestVertex& v) const noexcept {
            return _id == v._id;
        }
    };
}

namespace std {
    template <>
    struct hash<TestVertex> {
        std::size_t operator() (const TestVertex& v) const {
            return std::hash<int>{}(v._id);
        }
    };

    template <>
    struct formatter<TestVertex, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            return ctx.begin();
        }

        template <class FormatContext>
        FormatContext::iterator format(const TestVertex& v, FormatContext& ctx) const {
            return format_to(ctx.out(), "TestVertex[{id: {}, label: {}]", v._id, v._label);
        }
    };
}

// check if the concept is satisfied
static_assert(g::Vertex<TestVertex>);

TEST_CASE("Hello World", "[hello]")
{
    REQUIRE(1 == 1);
}
