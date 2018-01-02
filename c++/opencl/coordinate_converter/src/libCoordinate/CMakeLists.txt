#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    1/1/2017
#
#

project(LIB_COORDINATE)

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib )


#  Define the Sources
set( LIB_COORDINATE_SOURCES
        Coordinate.hpp
        Coordinate/Coordinate_Converter_Base.cpp
        Coordinate/Coordinate_Converter_Base.hpp
        Coordinate/Coordinate_Base.cpp
        Coordinate/Coordinate_Base.hpp
        Coordinate/Coordinate_Geographic.cpp
        Coordinate/Coordinate_Geographic.hpp
        Coordinate/Coordinate_UTM.cpp
        Coordinate/Coordinate_UTM.hpp
        Coordinate/Enumerations.cpp
        Coordinate/Enumerations.hpp
)


#  Build the library
add_library( Coordinate SHARED
             ${LIB_COORDINATE_SOURCES} )

