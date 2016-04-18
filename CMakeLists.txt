cmake_minimum_required (VERSION 2.6)

# Project declaration
project(woma)

# The version number.
set (woma_VERSION_MAJOR 0)
set (woma_VERSION_MINOR 1)

# Compiler Flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG")
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# C++11 Support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Look for Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# Look for armadillo
find_package(Armadillo)
if(ARMADILLO_FOUND)
    include_directories(${ARMADILLO_INCLUDE_DIRS})
    add_definitions(-DUSE_ARMA)
else(ARMADILLO_FOUND)
    message(WARNING "Armadillo not found. Deactivating Offline Computation Functions.")
endif(ARMADILLO_FOUND)

# Declare library
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

file(
    GLOB_RECURSE
    woma_source_files
    src/*
)

add_library(
    woma
    STATIC
    ${woma_source_files}
)

# linking configuration
if(ARMADILLO_FOUND)
    target_link_libraries(
        woma
        ${ARMADILLO_LIBRARIES}
    )
endif(ARMADILLO_FOUND)

# Declare Unit tests
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/catch)

file(
    GLOB_RECURSE
    woma_test_files
    test/*
)

add_executable(
    tests
    EXCLUDE_FROM_ALL
    ${woma_test_files}
)

# linking configuration
target_link_libraries(
    tests
    woma
)

set_target_properties(tests PROPERTIES OUTPUT_NAME woma_testing)
add_custom_command(TARGET tests POST_BUILD COMMAND ${EXECUTABLE_OUTPUT_PATH}/woma_testing)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)

find_program(DOTPATH dot)
if(${DOTPATH} STREQUAL "DOTPATH-NOTFOUND")
  set(HAS_DOT "")
else(${DOTPATH} STREQUAL "DOTPATH-NOTFOUND")
  set(HAS_DOT "-dot")
endif(${DOTPATH} STREQUAL "DOTPATH-NOTFOUND")

if(DOXYGEN_FOUND)
    add_custom_target(doc
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile${HAS_DOT}
                      DEPENDS ${tracking_source_files}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/
                      COMMENT "Generating API documentation with Doxygen" VERBATIM)
    add_custom_target(docapi
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile-API
                      DEPENDS ${tracking_source_files}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/
                      COMMENT "Generating API documentation with Doxygen" VERBATIM)
else(DOXYGEN_FOUND)
    message(STATUS "Doxygen not found, doc will not be included in the Makefile")
endif(DOXYGEN_FOUND)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/doc/html")
