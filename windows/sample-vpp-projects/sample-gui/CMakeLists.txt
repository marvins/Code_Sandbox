#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    9/29/2017
#
#    Purpose:  Base-Level CMake Build Script.
#

#  Set Required CMake Version
cmake_minimum_required( VERSION 3.0 )

#  Use C++11
set (CMAKE_CXX_STANDARD 14)


#------------------------------#
#-        Name Project        -#
#------------------------------#
project(Sample-GUI-Projects)


#-------------------------------------#
#-     Add Manual CMake Scripts      -#
#-------------------------------------#
LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")


#-------------------------------------#
#-      Add Project Dependencies     -#
#-------------------------------------#
add_subdirectory(src)
