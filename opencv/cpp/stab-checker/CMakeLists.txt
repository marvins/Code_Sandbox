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


#---------------------------#
#-   Find Thread Library   -#
#---------------------------#
find_package (Threads)

#----------------------------#
#-      Find NCurses        -#
#----------------------------#
find_package(Curses REQUIRED)

#--------------------#
#-    Find OpenCV   -#
#--------------------#
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDES} )


find_package( GDAL )

#-------------------#
#-    Find Boost   -#
#-------------------#
message("${Blue}Looking for Boost at ${BOOST_ROOT} ${ColourReset}")
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
set( APP_SOURCES
        stabilization-checker.cpp
        Interface.cpp
        Interface.hpp
        Log_Utilities.cpp
        Log_Utilities.hpp
        Options.cpp
        Options.hpp
        Point_Info.cpp
        Point_Info.hpp
        String_Utilities.cpp
        String_Utilities.hpp
)



#--------------------------------#
#-     Build the Executable     -#
#--------------------------------#
add_executable( opencv-video-stab-checker
                ${APP_SOURCES}
              )


#-------------------------------#
#-     Link the Executable     -#
#-------------------------------#
target_link_libraries( opencv-video-stab-checker
                       ${OpenCV_LIBRARIES}
                       ${CURSES_LIBRARIES}
                       ${CMAKE_THREAD_LIBS_INIT} )

