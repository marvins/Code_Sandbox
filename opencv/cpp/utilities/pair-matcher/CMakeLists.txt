#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    2/15/2016
#
#    Purpose: Build the C++ OpenCV Image Comparison Tool.
#


#  Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall")

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )


#--------------------#
#-    Find OpenCV   -#
#--------------------#
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDES} )


#-------------------#
#-    Find Boost   -#
#-------------------#
set( Boost_USE_STATIC_LIBS ON )
find_package(Boost REQUIRED
                COMPONENTS
                system
                filesystem
            )
include_directories(${Boost_INCLUDE_DIRS})

#----------------------------#
#-    Define the Sources    -#
#----------------------------#
set( PAIR_MATCHER_SOURCES
     pair-matcher.cpp
     Options.cpp
     Options.hpp
)



#--------------------------------#
#-     Build the Executable     -#
#--------------------------------#
add_executable( pair-matcher
                ${PAIR_MATCHER_SOURCES}
)



#-------------------------------#
#-     Link the Executable     -#
#-------------------------------#
target_link_libraries( pair-matcher
                       ${OpenCV_LIBRARIES}
                       ${Boost_LIBRARIES})

