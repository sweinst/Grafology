cmake_minimum_required(VERSION 3.25.0)
project(grafology VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)

message(STATUS "Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

find_package(fmt CONFIG REQUIRED)
find_package(catch2 CONFIG REQUIRED)

##################################################
# Libary
##################################################
set(GRAFOLOGY_SRC src/cpp/Grafology.cpp)
set(GRAFOLOGY_HDRS src/include/grafology/grafology.h)

add_library(${PROJECT_NAME} ${GRAFOLOGY_SRC} ${GRAFOLOGY_HDRS})
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${GRAFOLOGY_HDRS})

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
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)


install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# export(EXPORT ${PROJECT_NAME}Targets FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake
)

#install(
#    EXPORT ${PROJECT_NAME}Targets 
#    FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
#    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
#    NAMESPACE graphology::
#)

#install(
#    EXPORT grafologyTargets
#    FILE
#        ${CMAKE_CURRENT_BINARY_DIR}/grafologyConfig.cmake
#        ${CMAKE_CURRENT_BINARY_DIR}/grafologyConfigVersion.cmake
#        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/grafology
#)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
