#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    1/1/2017
#

#  Define the project
project(LIB_OPENCL_CONVERT_01)

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib )

#  Include Lib-Coordinate Headers
include_directories(../libCoordinate)

#  Define the sources
set( OPENCL_CONVERT_01_SOURCES
        OpenCL_Convert_02.cpp
        OpenCL_Convert_01.hpp
)


#  Build the library
add_library( OpenCLConvert01
             ${OPENCL_CONVERT_01_SOURCES} )

target_link_libraries( OpenCLConvert01
                        Coordinate )
