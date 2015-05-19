
#  Set the CMake Version
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

#   Set the Project Name
project(libcli-cpp)

#   Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall" )


#--------------------------------------#
#-      Find Boost C++ Libraries      -#
#--------------------------------------#
find_package( Boost REQUIRED
                COMPONENTS
                    system
                    filesystem
)

include_directories(${Boost_INCLUDE_DIRS})


#--------------------------------------#
#-       Find NCurses Libraries       -#
#--------------------------------------#
find_package( Curses REQUIRED )

include_directories(${CURSES_INCLUDE_DIR})

#   Define our sources
set( CLI_SOURCES
     src/cli/A_CLI_Configuration_File_Parser.cpp
     src/cli/A_CLI_Connection_Handler_Base.cpp
     src/cli/A_CLI_Connection_Handler_Base_Config.cpp
     src/cli/A_CLI_Connection_Handler_Local.cpp
     src/cli/A_CLI_Connection_Handler_Local_Config.cpp
     src/cli/A_CLI_Connection_Handler_Socket.cpp
     src/cli/A_CLI_Connection_Handler_Socket_Config.cpp
     src/cli/A_CLI_Manager.cpp
     src/cli/A_CLI_Manager_Configuration.cpp
     src/cli/A_CLI_Manager_Factory.cpp
     src/cli/A_Console_Render_Manager.cpp
     src/cli/CLIConnectionType.cpp
     src/thirdparty/ncurses/NCurses_Utilities.cpp
     src/thirdparty/pugixml/src/pugixml.cpp
)


#   Define our headers
set( CLI_HEADERS
     src/cli/A_CLI_Configuration_File_Parser.hpp
     src/cli/A_CLI_Connection_Handler_Base.hpp
     src/cli/A_CLI_Connection_Handler_Base_Config.hpp
     src/cli/A_CLI_Connection_Handler_Local.hpp
     src/cli/A_CLI_Connection_Handler_Local_Config.hpp
     src/cli/A_CLI_Connection_Handler_Socket.hpp
     src/cli/A_CLI_Connection_Handler_Socket_Config.hpp
     src/cli/A_CLI_Manager.hpp
     src/cli/A_CLI_Manager_Configuration.hpp
     src/cli/A_CLI_Manager_Factory.hpp
     src/cli/A_Console_Render_Manager.hpp
     src/cli/CLIConnectionType.hpp
     src/thirdparty/ncurses/NCurses_Utilities.hpp
     src/thirdparty/pugixml.hpp
     src/thirdparty/pugixml/src/pugixml.hpp
     src/thirdparty/pugixml/src/pugiconfig.hpp
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
                       ${Boost_LIBRARIES}
                       ${CURSES_LIBRARIES} )


#   Create the demo application
add_executable( cli_cpp_demo
                ${CLI_DEMO_MAIN}
)


#   Link
target_link_libraries( cli_cpp_demo 
                       cli_cpp
)


