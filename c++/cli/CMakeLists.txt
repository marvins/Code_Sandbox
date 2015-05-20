#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    5/20/2015
#
#    Purpose:  Constructs the CLI-CPP Project
#

#----------------------------------------#
#-        Set the CMake Version         -#
#----------------------------------------#
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )


#----------------------------------------#
#-          Set the Project Name        -#
#----------------------------------------#
project(libcli-cpp)


#------------------------------------#
#-     Add the Source Directory     -#
#------------------------------------#
add_subdirectory( src )

