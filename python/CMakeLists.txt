cmake_minimum_required (VERSION 2.6)

# Compiler Flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DJSON_DEBUG")
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

# Require global Woma project
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/_build_tmp)

# PYTHON STUFF
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
execute_process( COMMAND python -c "from distutils import sysconfig; print sysconfig.PREFIX" OUTPUT_VARIABLE PYTHON_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE )
execute_process( COMMAND python -c "import sys; a = sys.version; print a[:3]" OUTPUT_VARIABLE PYVERSION OUTPUT_STRIP_TRAILING_WHITESPACE )
if(PYTHON_PREFIX)
    if(EXISTS "${PYTHON_PREFIX}/lib/libpython${PYVERSION}.dylib")
        set(PYTHON_LIBRARY "${PYTHON_PREFIX}/lib/libpython${PYVERSION}.dylib")
        set(PYTHON_INCLUDE_DIR "${PYTHON_PREFIX}/Headers")
    endif(EXISTS "${PYTHON_PREFIX}/lib/libpython${PYVERSION}.dylib")
endif(PYTHON_PREFIX)
find_package(PythonLibs)
message(STATUS "PYTHON_LIBRARIES: ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_INCLUDE_PATH: ${PYTHON_INCLUDE_PATH}")
message(STATUS "PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS}")
message(STATUS "PYTHONLIBS_VERSION_STRING: ${PYTHONLIBS_VERSION_STRING}")

## Try to determine the directory of numpy headers
execute_process( COMMAND python -c "from numpy import distutils; path=distutils.misc_util.get_numpy_include_dirs(); print path[0]" OUTPUT_VARIABLE NUMPY_INCLUDEDIR OUTPUT_STRIP_TRAILING_WHITESPACE )
if (NOT NUMPY_INCLUDE)
    SET(NUMPY_INCLUDE ${NUMPY_INCLUDEDIR} CACHE PATH "Directory for the numpy headers. If it is not set automatically it is determined by python distutils." )
endif( NOT NUMPY_INCLUDE)
include_directories( ${NUMPY_INCLUDE} )
if( NOT EXISTS ${NUMPY_INCLUDE}/numpy/arrayobject.h )
    message( FATAL_ERROR "${NUMPY_INCLUDE}/numpy/arrayobject.h header is not found. If it is installed, than check the python flags and numpy header directories") 
endif( NOT EXISTS ${NUMPY_INCLUDE}/numpy/arrayobject.h )

# Look for armadillo
find_package(Armadillo)
if(ARMADILLO_FOUND)
    include_directories(${ARMADILLO_INCLUDE_DIRS})
    add_definitions(-DUSE_ARMA)
endif(ARMADILLO_FOUND)

# Include python and Woma header directories
include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/wavelets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/armanpy)

# Doxygen XML documentation
find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(XML_DOC_INDEX ${CMAKE_CURRENT_SOURCE_DIR}/_build_tmp/xml/index.xml)
    add_custom_command(OUTPUT ${XML_DOC_INDEX}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src/Doxyfile-Python
                      DEPENDS ${tracking_source_files}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
                      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_custom_target(docpy DEPENDS ${XML_DOC_INDEX})
endif(DOXYGEN_FOUND)

# Add target for python documentation from doxygen
file(
    GLOB_RECURSE
    woma_header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.h
)
add_custom_command(OUTPUT woma_doc.i
                   DEPENDS docpy ${woma_header_files}
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/src/doxy2swig.py ${CMAKE_CURRENT_SOURCE_DIR}/_build_tmp/xml/index.xml ${CMAKE_CURRENT_SOURCE_DIR}/woma_doc.i)

# Look for armadillo
set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/numpy.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/woma.i PROPERTIES CPLUSPLUS ON)
if(ARMADILLO_FOUND)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/woma.i PROPERTIES SWIG_FLAGS "-DUSE_ARMA")
endif(ARMADILLO_FOUND)
SET(SWIG_MODULE_woma_EXTRA_DEPS 
    woma_doc.i
)
SWIG_ADD_MODULE(woma python ${CMAKE_CURRENT_SOURCE_DIR}/src/woma.i)
SWIG_LINK_LIBRARIES(woma ${PYTHON_LIBRARIES} woma)

add_custom_command(TARGET _woma POST_BUILD COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/src/move_files.sh)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/bin/woma;${CMAKE_CURRENT_SOURCE_DIR}/_build_tmp/xml")
