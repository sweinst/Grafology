#include <grafology/graph.h>
#include <print>

namespace g = grafology;

int main()
{
    g::SparseGraphImpl graph(10, 10, true);
    std::println("Graph capacity: {}", graph.capacity());
    return 0;
}

