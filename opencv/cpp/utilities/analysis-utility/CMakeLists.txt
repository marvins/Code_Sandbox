#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    2/12/2016
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
set( ANALYSIS_TOOL_SOURCES
     analysis-utility.cpp
     Options.cpp
     Options.hpp
)



#--------------------------------#
#-     Build the Executable     -#
#--------------------------------#
add_executable( analysis-utility
                ${ANALYSIS_TOOL_SOURCES}
)



#-------------------------------#
#-     Link the Executable     -#
#-------------------------------#
target_link_libraries( analysis-utility
                       ${OpenCV_LIBRARIES}
                       ${Boost_LIBRARIES})

