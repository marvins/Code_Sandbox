#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    1/13/2017
#

#  Add Libraries
include_directories( ../src/libCoordinate
                     ../src/libConvert01
                     ../thirdparty/googletest/googletest/include
                     ../thirdparty/pugixml/src )

#  Define Source Files
set( UNIT_TEST_SOURCES
        TEST_libConvert01.cpp )


#  Create the unit-test executable
add_executable( coordinate-test
                coordinate-test.cpp
                Coordinate_Test_Data.cpp
                Coordinate_Test_Data.hpp
                TEST_libConvert01.cpp )

target_link_libraries( coordinate-test
                        Coordinate
                        OpenCLConvert01
                        gmock
                        pugixml )