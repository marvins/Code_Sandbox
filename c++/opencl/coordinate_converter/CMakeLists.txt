#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    1/1/2017
#
#  Set the Required CMake Version
cmake_minimum_required( VERSION 3.0 FATAL_ERROR )


project(OpenCL_Coordinate_Converter)


#  Setup the Compiler
#  Add C++ 11 Support
SET(CMAKE_CXX_STANDARD 11)

#  Find OpenCL
find_package( OpenCL REQUIRED )

#  Define the thirdparty libraries
add_subdirectory(thirdparty)

#  Define the library sources
add_subdirectory(src)

#  Add Tests
add_subdirectory(tests)
