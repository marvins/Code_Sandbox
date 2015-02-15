
#  Define Required CMake Version
cmake_minimum_required( VERSION 2.8.9 FATAL_ERROR )

# Define the project name
project(edid-parser)

#  Allow C++ 11x
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall")



set( SOURCES
     src/main.cpp
     src/EDID_Container.cpp
     src/String_Utilities.cpp
)

set( HEADERS
     src/EDID_Container.hpp
     src/String_Utilities.hpp
)


add_executable( edid-parser
                ${SOURCES}
                ${HEADERS}
              )


