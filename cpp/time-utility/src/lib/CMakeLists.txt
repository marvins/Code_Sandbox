#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    10/19/2015
#
#    Purpose: Construct the Library
#


#  Configure C++ Options
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

#------------------------------------------#
#-     Override the Installation Paths    -#
#------------------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

#---------------------------------------#
#-      Set Utility Source Files       -#
#---------------------------------------#
set( TIME_UTILITY_LIB_SOURCES
    File_Utilities.cpp
    File_Utilities.hpp
    GPS_Time.cpp
    GPS_Time.hpp
    Leap_Second_Table.cpp
    Leap_Second_Table.hpp
    Time_Constants.hpp
    Time_Conversions.cpp
    Time_Conversions.hpp
)


#---------------------------------#
#-      Build the Executable     -#
#---------------------------------#
add_library(time_utility STATIC
            ${TIME_UTILITY_LIB_SOURCES}
)

