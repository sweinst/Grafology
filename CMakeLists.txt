set (GRAFOLOGY_VERSION 0.1.0)

cmake_minimum_required(VERSION 3.25.0)
project(Grafology VERSION ${GRAFOLOGY_VERSION} LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

message(STATUS "Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

find_package(fmt CONFIG REQUIRED)
find_package(catch2 CONFIG REQUIRED)

set(GRAFOLOGY_SRC src/cpp/Grafology.cpp)
set(GRAFOLOGY_HDRS src/include/grafology/grafology.h)

add_library(grafology ${GRAFOLOGY_SRC} ${GRAFOLOGY_HDRS})
target_link_libraries(grafology PRIVATE fmt::fmt-header-only)
target_include_directories(grafology PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set_target_properties(grafology PROPERTIES VERSION ${GRAFOLOGY_VERSION})
set_target_properties(grafology PROPERTIES PUBLIC_HEADER ${GRAFOLOGY_HDRS})

##################################################
# Tests
##################################################
add_subdirectory(tests)

##################################################
# Installation
##################################################
include(CMakePackageConfigHelpers)
configure_package_config_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/grafologyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/grafologyConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/grafology
)


write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/grafologyConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    TARGETS grafology
    EXPORT grafologyTargets
    LIBRARY
    COMPONENT grafology-lib
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
PUBLIC_HEADER
    COMPONENT grafology-dev
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/grafology
)

export(EXPORT grafologyTargets FILE ${CMAKE_CURRENT_BINARY_DIR}/grafologyTargets.cmake)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/grafologyConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/grafologyConfigVersion.cmake
    COMPONENT grafology-dev
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/grafology
)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
