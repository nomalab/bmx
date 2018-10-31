cmake_minimum_required(VERSION 2.8)

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release FORCE )
endif()

project (cbmx)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("/usr/local/include")
include_directories("/usr/local/include/bmx-0.1")
include_directories("/usr/local/include/libMXF++-1.0")
include_directories("/usr/local/include/libMXF-1.0")

add_subdirectory (src)
