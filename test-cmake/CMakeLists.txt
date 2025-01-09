cmake_minimum_required(VERSION 3.25.0)
project(test-cmake VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    set(DIST_OS "windows")
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux")
    set(DIST_OS "linux")
elseif ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
    set(DIST_OS "osx")
endif()

set(OS_PREFIX ${})
list (APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../dist/${DIST_OS}/cmake")
message(STATUS "=> ${CMAKE_PREFIX_PATH}")

find_package(grafology CONFIG REQUIRED)
add_executable(test-cmake test.cpp)
target_link_libraries(test-cmake PRIVATE grafology)
