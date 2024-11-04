project(Grafology-Tests LANGUAGES C CXX)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE Grafology Catch2::Catch2WithMain fmt::fmt-header-only)

include(CTest)
include(Catch)
catch_discover_tests(tests)
