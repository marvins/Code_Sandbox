#    Name:    Marvin Smith
#    Date:    11/3/2024
#

#  Set CMake Version
cmake_minimum_required( VERSION 3.22 FATAL_ERROR )

# Set the Project-Name
project( memtracker
         LANGUAGES CXX )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Setup libdl for debug GCC builds
if( CMAKE_BUILD_TYPE STREQUAL "debug" OR CMAKE_BUILD_TYPE STREQUAL "Debug" )
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -ldl" )
    endif()
endif()


find_package( Boost REQUIRED )
include_directories( ${Boost_INCLUDE_DIRS} )

include_directories( src )


add_executable( ${PROJECT_NAME}
                src/main.cpp
                src/memtracker/MemoryTracker.hpp 
                src/memtracker/Result.hpp )

target_link_libraries( ${PROJECT_NAME}
                       boost::boost )

message( "CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")