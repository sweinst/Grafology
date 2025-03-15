project(Grafology-examples LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

set(common_src
    london_tube.h
    london_tube.cpp
    utils.cpp
    utils.h
)

set (examples
    ex01-topological-sort-graph
    ex01-topological-sort-impl
    ex02-depth-and-breath-first-search-graph
    ex02-depth-and-breath-first-search-impl
    ex03-minimal-spanning-tree
    ex04-shortest-paths
)

if(WIN32)
	set(PYTHON3 python)
else()
	set(PYTHON3 python3)
endif(WIN32)

add_custom_command(
		OUTPUT  "${CMAKE_CURRENT_SOURCE_DIR}/fx_rates.h"
		COMMAND ${PYTHON3}
			"${CMAKE_CURRENT_SOURCE_DIR}/get_fx_rates.py"
            "-o" "${CMAKE_CURRENT_SOURCE_DIR}/fx_rates.h"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/get_fx_rates.py"
)

foreach(example ${examples})
    add_executable(${example} ${example}.cpp ${common_src})
    target_link_libraries(${example} PRIVATE grafology) 
endforeach()

set (FX_ARBITRAGE_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/fx_rates.h"
    fx_arbitrage.cpp
    fx_rates.h
)

add_executable(fx_arbitrage ${FX_ARBITRAGE_SRCS})
target_link_libraries(fx_arbitrage PRIVATE grafology) 
