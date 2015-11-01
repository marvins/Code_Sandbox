#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    10/30/2015
#
#    Purpose: Build the C++ OpenCV Video Writer
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

#----------------------------#
#-    Define the Sources    -#
#----------------------------#
set( VIDEO_WRITER_SOURCES
     Options.cpp
     Options.hpp
     writer.cpp
)



#--------------------------------#
#-     Build the Executable     -#
#--------------------------------#
add_executable( video-writer
                ${VIDEO_WRITER_SOURCES}
)



#-------------------------------#
#-     Link the Executable     -#
#-------------------------------#
target_link_libraries( video-writer
                       ${OpenCV_LIBRARIES})

