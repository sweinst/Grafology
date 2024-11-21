project(Grafology-Tests LANGUAGES C CXX)

add_executable(
    tests 
    test_vertex.h 
    tests_algos.cpp
    tests_bimap.cpp
    tests_flat_index_map.cpp
    tests_graph.cpp
    tests_graph_impl.cpp
)
target_link_libraries(tests PRIVATE grafology Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
