cmake_minimum_required(VERSION 3.25.0)
project(test-cmake VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
list (APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../dist/Windows/Install/Debug/include")
message(STATUS "=> ${CMAKE_PREFIX_PATH}")

find_package(grafology CONFIG REQUIRED)
add_executable(test-cmake test.cpp)
target_link_libraries(test-cmake PRIVATE grafology)
