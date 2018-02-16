#   File:    CMakeLists.txt
#   Author:  Marvin Smith
#   Date:    2/15/2018


cmake_minimum_required( VERSION 3.0 FATAL_ERROR )


#   Set the project name
PROJECT( libVNC-Demos )


#-------------------------------------#
#-     Add Manual CMake Scripts      -#
#-------------------------------------#
LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")


add_subdirectory(simple-server)


