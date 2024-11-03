#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    1/1/2017
#

#  Define the project
project(LIB_OPENCL_CONVERT_02)

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib )

#  Include Lib-Coordinate Headers
include_directories(../libCoordinate)

find_package( OpenCL REQUIRED )

#  Define the sources
set( OPENCL_CONVERT_02_SOURCES
        OpenCL_Convert_02.cpp
        OpenCL_Convert_02.hpp
        OpenCL_Utilities.cpp
        OpenCL_Utilities.hpp
)


#  Build the library
add_library( OpenCLConvert02
             ${OPENCL_CONVERT_02_SOURCES} )

target_link_libraries( OpenCLConvert02
                        Coordinate
                        ${OpenCL_LIBRARY} )
