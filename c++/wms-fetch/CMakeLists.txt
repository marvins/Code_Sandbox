#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    1/27/2015
#
#    Purpose: Construct the WMS Fetch Test Application
#


#  Set Minimum CMake Version
cmake_minimum_required(  VERSION 2.8 FATAL_ERROR )

#   Set the project name
PROJECT( WMS_Fetch )

#  Add C++11x Support
SET( CMAKE_CXX_FLAGS "-std=c++11 -Wall ")


#  Find Curl
find_package(CURL)



