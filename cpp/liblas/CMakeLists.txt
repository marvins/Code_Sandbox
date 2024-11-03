

#-------------------------------------------------------#
#-       Set the Minimum Required CMake Version        -#
#-------------------------------------------------------#
cmake_minimum_required( VERSION 2.6 FATAL_ERROR )

#----------------------------#
#-        Allow C++ 11x     -#
#----------------------------#
SET(CMAKE_CXX_FLAGS "-std=c++11")

#-------------------#
#-    Find Boost   -#
#-------------------#
find_package( Boost 
                COMPONENTS
                    system
                    filesystem
                REQUIRED
)
include_directories(${Boost_INCLUDE_DIRS})
if( NOT ${Boost_FOUND} )
    MESSAGE( FATAL_ERROR "Unable to find Boost Libraries.")
endif()

#------------------------#
#-     Find OpenCV      -#
#------------------------#
Find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} ) 

#-----------------------#
#-     Find LibLas     -#
#-----------------------#
Find_Library( LIBLAS_LIBRARY
                NAME
                    las
                PATHS
                    /opt/local/lib
)

FIND_PATH( LIBLAS_INCLUDE
                NAME
                    liblas.hpp
                HINTS
                    /opt/local/include
                    /opt/local/include/liblas
)

message("liblas include dir: ${LIBLAS_INCLUDE}")
message("liblas library    : ${LIBLAS_LIBRARY}")
include_directories(${LIBLAS_INCLUDE})


#------------------------------#
#-     Define our sources     -#
#------------------------------#
set( DEMO_SOURCES
    src/main.cpp
    src/GridBuilder.hpp
    src/GridBuilder.cpp
    src/GridMarker.hpp
    src/GridMarker.cpp
    src/PointExtractor.hpp
    src/PointExtractor.cpp
)


add_executable( point-extractor
                ${DEMO_SOURCES}
)

target_link_libraries( point-extractor
                       ${Boost_LIBRARIES}
                       ${LIBLAS_LIBRARY}
                       ${OpenCV_LIBRARIES}
)

