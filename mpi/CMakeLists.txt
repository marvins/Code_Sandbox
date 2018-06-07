#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    6/6/2018
#


#  Set the CMake Version
cmake_minimum_required( VERSION 3.0 FATAL_ERROR )

#   Set the project name
PROJECT( MPI_Demos )

#  Define some common files
set( COMMON_SOURCES
        ${CMAKE_SOURCE_DIR}/common/Print_Utilities.hpp
        ${CMAKE_SOURCE_DIR}/common/Print_Utilities.cpp
)


#  Add Hello World Applications
add_subdirectory(hello_world)

#  Add Dynamic Window Applications
add_subdirectory(dynamic-windows)

#  Add Fault-Tolerance Applications
add_subdirectory(fault-tolerance)

#  Add Other Demos
add_subdirectory(other_examples)

