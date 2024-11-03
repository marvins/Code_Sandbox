#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    8/2/2024
#
#    Build C++ 23 demos

#  Set CMake Version
cmake_minimum_required( VERSION 3.22 FATAL_ERROR )

# Set the Project-Name
project( cpp-23-demos )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#  Set C++ 20 Support
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_executable( spanstream-demo
                spanstream-demo.cpp )