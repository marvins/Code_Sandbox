
#  Set the CMake Version
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

#   Set the Project Name
project(libcli-cpp)

#   Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11" )


#--------------------------------------#
#-      Find Boost C++ Libraries      -#
#--------------------------------------#
find_package( Boost REQUIRED
                COMPONENTS
                    system
                    filesystem
)

include_directories(${Boost_INCLUDE_DIRS})


#   Define our sources
set( CLI_SOURCES
     src/cli/A_CLI_Configuration_File_Parser.cpp
     src/cli/A_CLI_Manager.cpp
     src/cli/A_CLI_Manager_Configuration.cpp
     src/cli/A_CLI_Manager_Factory.cpp
)


#   Define our headers
set( CLI_HEADERS
     src/cli/A_CLI_Configuration_File_Parser.hpp
     src/cli/A_CLI_Manager.hpp
     src/cli/A_CLI_Manager_Configuration.hpp
     src/cli/A_CLI_Manager_Factory.hpp
)

     
#   Define our main files
set( CLI_DEMO_MAIN
     src/main.cpp
)

#   Create the Library
add_library( cli_cpp
             ${CLI_HEADERS}
             ${CLI_SOURCES}
)

target_link_libraries( cli_cpp 
                       ${Boost_LIBRARIES} )


#   Create the demo application
add_executable( cli_cpp_demo
                ${CLI_DEMO_MAIN}
)


#   Link
target_link_libraries( cli_cpp_demo 
                       cli_cpp
)


