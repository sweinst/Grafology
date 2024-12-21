project(Grafology-Tests LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

add_executable(
    tests 
    test_vertex.h 
    tests_algos_graph.cpp
    tests_algos_impl.cpp
    tests_bimap.cpp
    tests_disjoint_set.cpp
    tests_flat_index_map.cpp
    tests_generator.cpp
    tests_graph.cpp
    tests_graph_impl.cpp
)
target_link_libraries(tests PRIVATE grafology Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
