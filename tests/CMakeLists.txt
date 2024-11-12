project(Grafology-Tests LANGUAGES C CXX)

add_executable(tests tests_vertex.cpp)
target_link_libraries(tests PRIVATE grafology Catch2::Catch2WithMain fmt::fmt-header-only)

include(CTest)
include(Catch)
catch_discover_tests(tests)
