#include <grafology/grafology.h>
//#include <fmt/core.h>

namespace g = grafology;

namespace{
    struct TestVertex{

        int _id;
        std::string _label;

        bool operator == (const TestVertex& v) const {
            return _id == v._id;
        }
    }

    template <>
    std::hash<TestVertex> {
        std::size_t operator()(const TestVertex& v) const {
            return std::hash<int>{}(v._id);
        }
    };

    static_assert(g::Vertex<TestVertex>);
}

int main()
{
    g::test();
    return 0;
}
