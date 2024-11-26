project(Grafology-examples LANGUAGES C CXX)

set (examples
    ex01-topological-sort-graph
    ex01-topological-sort-impl
    ex02-depth-and-breath-first-search-graph
    ex02-depth-and-breath-first-search-impl
)

foreach(example ${examples})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} PRIVATE grafology) 
endforeach()
