#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    6/6/2018
#


#  Set the CMake Version
cmake_minimum_required( VERSION 3.0 FATAL_ERROR )

#   Set the project name
PROJECT( MPI_Demos )

#  Add Hello World Applications
add_subdirectory(hello_world)

#  Add Dynamic Window Applications
add_subdirectory(dynamic-windows)

#  Add Fault-Tolerance Applications
add_subdirectory(fault-tolerance)

