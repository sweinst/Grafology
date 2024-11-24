project(Grafology-examples LANGUAGES C CXX)

add_executable(
    ex01-topological-sort
    ex01-topological-sort.cpp
)
target_link_libraries(ex01-topological-sort PRIVATE grafology)
