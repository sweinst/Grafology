cmake_minimum_required(VERSION 3.26)
project(grafology VERSION 0.1.0 LANGUAGES C CXX)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(CODE_COVERAGE "Set up code coverage" OFF)

message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Build examples: ${BUILD_EXAMPLES}")
message(STATUS "Code coverage: ${CODE_COVERAGE}")

if (CODE_COVERAGE)
    if (LINUX)
        add_compile_options(--coverage)
        add_link_options(--coverage)
    else(LINUX)
        message(FATAL_ERROR "Code coverage is not supported on this platform")
    endif(LINUX)
endif(CODE_COVERAGE)

set(CMAKE_CXX_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

message(STATUS "Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

if(BUILD_TESTS)
    find_package(Catch2 CONFIG REQUIRED)
endif(BUILD_TESTS)

##################################################
# Library
##################################################
set(GRAFOLOGY_SRC 
    src/cpp/Grafology.cpp
    src/cpp/sparse_graph_impl.cpp
    src/cpp/dense_graph_impl.cpp
    src/cpp/flat_index_map.cpp
)

set(GRAFOLOGY_HDRS 
    src/include/grafology/bimap.h
    src/include/grafology/dense_graph_impl.h
    src/include/grafology/disjoint_set.h
    src/include/grafology/flat_index_map.h
    src/include/grafology/graph.h 
    src/include/grafology/graph_impl.h
    src/include/grafology/sparse_graph_impl.h
    src/include/grafology/vertex.h 
    src/include/grafology/algorithms/all_shortest_paths.h
    src/include/grafology/algorithms/articulation_points.h
    src/include/grafology/algorithms/breath_first_search.h
    src/include/grafology/algorithms/bridges.h
    src/include/grafology/algorithms/depth_first_search.h
    src/include/grafology/algorithms/maximum_flow.h
    src/include/grafology/algorithms/minimum_spanning_tree.h
    src/include/grafology/algorithms/requirements.h
    src/include/grafology/algorithms/shortest_path.h
    src/include/grafology/algorithms/topological_sort.h
    src/include/grafology/algorithms/transitive_closure.h
    src/include/grafology/internal/generator.h
)

add_library(${PROJECT_NAME} ${GRAFOLOGY_SRC} ${GRAFOLOGY_HDRS})
target_include_directories (${PROJECT_NAME} INTERFACE
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include> 
    PUBLIC $<INSTALL_INTERFACE:include>
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES 
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${GRAFOLOGY_HDRS}"
)

##################################################
# Tests
##################################################
if(BUILD_TESTS)
    # this must be done before add_subdirectory (see enable_testing documentation)
    include (CTest)
    enable_testing()

    add_subdirectory(tests)
endif(BUILD_TESTS)

##################################################
# Examples
##################################################
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

##################################################
# Installation
##################################################
include(CMakePackageConfigHelpers)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib/${CMAKE_BUILD_TYPE}
)

# NB: the final slash is important as it will allow to preserve the directory structure
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/include/" # source directory
        DESTINATION "include" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
)

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}-config.cmake
    DESTINATION cmake
)

write_basic_package_version_file(
    ${PROJECT_NAME}-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION cmake
)
