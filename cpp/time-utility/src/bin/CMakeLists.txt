#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    10/19/2015
#
#    Purpose: Construct the Application
#


#  Configure C++ Options
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

#------------------------------------------#
#-     Override the Installation Paths    -#
#------------------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )


#--------------------------------------------#
#-     Include the Library Header Files     -#
#--------------------------------------------#
include_directories(../lib)

#---------------------------------#
#-      Set the Main Driver      -#
#---------------------------------#
set( TIME_UTILITY_MAIN
     time-utility.cpp
)


#---------------------------------------#
#-      Set Utility Source Files       -#
#---------------------------------------#
set( TIME_UTILITY_SOURCES
     Globals.cpp
     Globals.hpp
     Options.cpp
     Options.hpp
     Print_Times.cpp
     Print_Times.hpp
)


#---------------------------------#
#-      Build the Executable     -#
#---------------------------------#
add_executable(time-utility
               ${TIME_UTILITY_MAIN}
               ${TIME_UTILITY_SOURCES}
)

#---------------------------------------#
#-      Link against dependencies      -#
#---------------------------------------#
target_link_libraries( time-utility
                       time_utility
)

