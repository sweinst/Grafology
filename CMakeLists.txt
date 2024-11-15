cmake_minimum_required(VERSION 3.26)
project(grafology VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

message(STATUS "Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

find_package(Catch2 CONFIG REQUIRED)

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
    src/include/grafology/grafology.h
    src/include/grafology/vertex.h 
    src/include/grafology/graph.h 
    src/include/grafology/graph_impl.h
    src/include/grafology/sparse_graph_impl.h
    src/include/grafology/dense_graph_impl.h
    src/include/grafology/flat_index_map.h
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
add_subdirectory(tests)

##################################################
# Installation
##################################################
include(CMakePackageConfigHelpers)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib/${CMAKE_BUILD_TYPE}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
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
