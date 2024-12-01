#include <grafology/graph.h>
#include <iostream>

namespace g = grafology;

struct TestVertex{
    int _id;
    std::string _label {};

    bool operator == (const TestVertex& v) const noexcept {
        return _id == v._id;
    }
};

// for Catch2 which uses streams
inline std::ostream& operator << (std::ostream& os, const TestVertex& v) {
    return os << "TestVertex[" << v._id << ",'" << v._label << "']";
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
        auto format(const TestVertex& v, FormatContext& ctx) const {
            return std::format_to(ctx.out(), "TestVertex[id: {}, label: {}]", v._id, v._label);
        }
    };
}

// check if the concept is satisfied
static_assert(g::VertexKey<TestVertex>);

