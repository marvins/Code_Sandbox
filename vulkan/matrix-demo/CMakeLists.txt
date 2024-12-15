#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    12/14/2024
#
#    Purpose: Build the Vulkan demo
#
cmake_minimum_required(VERSION 3.10)
project( matrix-demo )

find_package(Vulkan REQUIRED)

set(SOURCES
    src/main.cpp
    src/core/Context.cpp
    src/core/Context.hpp
)

add_executable( matrix-demo ${SOURCES} )

set(CMAKE_BUILD_TYPE Debug) # Ensure debug build
set(CMAKE_CXX_FLAGS_DEBUG "-g") # Add debug symbols

target_include_directories( matrix-demo PRIVATE ${Vulkan_INCLUDE_DIRS} )
target_link_libraries( matrix-demo ${Vulkan_LIBRARIES} )
