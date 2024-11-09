cmake_minimum_required(VERSION 3.26.0)
project(grafology VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

message(STATUS "Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

find_package(fmt CONFIG REQUIRED)
find_package(catch2 CONFIG REQUIRED)

##################################################
# Libary
##################################################
set(GRAFOLOGY_SRC src/cpp/Grafology.cpp)
set(GRAFOLOGY_HDRS src/include/grafology/grafology.h)

add_library(${PROJECT_NAME} ${GRAFOLOGY_SRC} ${GRAFOLOGY_HDRS})
target_include_directories (${PROJECT_NAME} INTERFACE
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include> 
    PUBLIC $<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER ${GRAFOLOGY_HDRS}
)

# as we don't want fmt to be a dependency of the library, we can't use target_link_libraries
# i.e.: target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
# so let's use our own version
set_target_properties(${PROJECT_NAME} PROPERTIES
  COMPILE_OPTIONS "\$<\$<AND:\$<COMPILE_LANGUAGE:CXX>,\$<CXX_COMPILER_ID:MSVC>>:/utf-8;/EHsc>"
  COMPILE_DEFINITIONS "FMT_HEADER_ONLY=1"
)
target_include_directories(${PROJECT_NAME} PRIVATE $<TARGET_PROPERTY:fmt::fmt-header-only,INTERFACE_INCLUDE_DIRECTORIES>)

##################################################
# Tests
##################################################
add_subdirectory(tests)

##################################################
# Installation
##################################################
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

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
