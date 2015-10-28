#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    10/27/2015
#
#    Purpose: Build the simple stitcher
#

#  Set the Minimum Required Version of CMake
cmake_minimum_required( VERSION 3.0 FATAL_ERROR )

#  Define the Project
project(simple-stitcher)

#  Set C++11 Support
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}" )

#  Find Eigen
find_package(Eigen3 REQUIRED)
include_directories( ${EIGEN3_INCLUDE_DIRS} )


#  Find Boost
find_package(Boost 
                COMPONENTS    
                    SYSTEM
                    FILESYSTEM
                REQUIRED
            )
include_directories( ${Boost_INCLUDE_DIRS} )


#  Find GDAL
find_package( GDAL REQUIRED )
include_directories( ${GDAL_INCLUDE_DIR} )


#-----------------------------------#
#-    Define the Project Sources   -#
#-----------------------------------#
set( SIMPLE_STITCH_SOURCES
     src/main.cpp
     src/Frame_Set.cpp
     src/Frame_Set.hpp
     src/Options.cpp
     src/Options.hpp
)


#------------------------------#
#-    Build the Executable    -#
#------------------------------#
add_executable( simple-stitch 
                ${SIMPLE_STITCH_SOURCES}
)                


#--------------------------#
#-    Link the Library    -#
#--------------------------#
target_link_libraries( simple-stitch
                       ${Boost_LIBRARIES}
                       ${GDAL_LIBRARY}
)


