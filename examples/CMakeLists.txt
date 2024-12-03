project(Grafology-examples LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

set(common_src
    london_tube.h
    london_tube.cpp
)

set (examples
    ex01-topological-sort-graph
    ex01-topological-sort-impl
    ex02-depth-and-breath-first-search-graph
    ex02-depth-and-breath-first-search-impl
    ex03-minimal-spanning-tree
    ex04-all-shortest.paths
)

foreach(example ${examples})
    add_executable(${example} ${example}.cpp ${common_src})
    target_link_libraries(${example} PRIVATE grafology) 
endforeach()
