cmake_minimum_required(VERSION 3.25.0)
project(Grafology VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

message(STATUS "Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

find_package(fmt CONFIG REQUIRED)
find_package(catch2 CONFIG REQUIRED)

add_library(Grafology Grafology.cpp)
target_link_libraries(Grafology PRIVATE fmt::fmt-header-only)

add_subdirectory(tests)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
