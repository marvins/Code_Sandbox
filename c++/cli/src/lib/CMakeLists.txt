#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    5/20/2015
#
#    Purpose:  Constructs the CLI-CPP Library Project
#

#   Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -ldl" )

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

#-------------------------------------#
#-     Find Boost C++ Libraries      -#
#-------------------------------------#
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package( Boost REQUIRED
                COMPONENTS
                    system
                    filesystem
                    regex
                    log
                    thread
)

include_directories( ${Boost_INCLUDE_DIRS})



#--------------------------------------#
#-       Find NCurses Libraries       -#
#--------------------------------------#
find_package( Curses REQUIRED )

include_directories(${CURSES_INCLUDE_DIR})


#--------------------------------------------#
#-       Define the Library Sources         -#
#--------------------------------------------#
set( CLI_CPP_SOURCES 
     cli_cpp/cli/A_CLI_Command_Queue.cpp
     cli_cpp/cli/A_CLI_Configuration_File_Parser.cpp
     cli_cpp/cli/A_CLI_Connection_Handler_Base.cpp
     cli_cpp/cli/A_CLI_Connection_Handler_Base_Config.cpp
     cli_cpp/cli/A_CLI_Connection_Handler_Local.cpp
     cli_cpp/cli/A_CLI_Connection_Handler_Local_Config.cpp
     cli_cpp/cli/A_CLI_Connection_Handler_Socket.cpp
     cli_cpp/cli/A_CLI_Connection_Handler_Socket_Config.cpp
     cli_cpp/cli/A_CLI_Manager.cpp
     cli_cpp/cli/A_CLI_Manager_Configuration.cpp
     cli_cpp/cli/A_CLI_Manager_Factory.cpp
     cli_cpp/cli/A_Command_History_Entry.cpp
     cli_cpp/cli/A_Command_Response_Handler_Base.cpp
     cli_cpp/cli/A_Console_Render_Manager.cpp
     cli_cpp/cli/A_Console_Render_Manager_ASCII.cpp
     cli_cpp/cli/A_Console_Render_Manager_NCurses.cpp
     cli_cpp/cli/A_Console_Render_State.cpp
     cli_cpp/cli/CLIConnectionType.cpp
     cli_cpp/cli/cmd/A_CLI_Command.cpp
     cli_cpp/cli/cmd/A_CLI_Command_Argument.cpp
     cli_cpp/cli/cmd/A_CLI_Command_Parser.cpp
     cli_cpp/cli/cmd/A_CLI_Command_Parser_Factory.cpp
     cli_cpp/cli/cmd/A_CLI_Command_Result.cpp
     cli_cpp/cli/cmd/A_CLI_Parser_Command.cpp
     cli_cpp/cli/cmd/CLICommandArgumentType.cpp
     cli_cpp/cli/cmd/CLICommandParseStatus.cpp
     cli_cpp/thirdparty/ncurses/An_NCurses_Table.cpp
     cli_cpp/thirdparty/ncurses/NCurses_Utilities.cpp
     cli_cpp/thirdparty/pugixml/src/pugixml.cpp
     cli_cpp/utils/Log_Utilities.cpp
     cli_cpp/utils/String_Utilities.cpp
     cli_cpp/utils/System_Utilities.cpp
)


#-----------------------------------------#
#-      Define the Library Headers       -#
#-----------------------------------------#
set( CLI_CPP_HEADERS
     cli_cpp/cli/A_CLI_Command_Queue.hpp
     cli_cpp/cli/A_CLI_Configuration_File_Parser.hpp
     cli_cpp/cli/A_CLI_Connection_Handler_Base.hpp
     cli_cpp/cli/A_CLI_Connection_Handler_Base_Config.hpp
     cli_cpp/cli/A_CLI_Connection_Handler_Local.hpp
     cli_cpp/cli/A_CLI_Connection_Handler_Local_Config.hpp
     cli_cpp/cli/A_CLI_Connection_Handler_Socket.hpp
     cli_cpp/cli/A_CLI_Connection_Handler_Socket_Config.hpp
     cli_cpp/cli/A_CLI_Manager.hpp
     cli_cpp/cli/A_CLI_Manager_Configuration.hpp
     cli_cpp/cli/A_CLI_Manager_Factory.hpp
     cli_cpp/cli/A_Command_History_Entry.hpp
     cli_cpp/cli/A_Command_Response_Handler_Base.hpp
     cli_cpp/cli/A_Console_Render_Manager.hpp
     cli_cpp/cli/A_Console_Render_Manager_ASCII.hpp
     cli_cpp/cli/A_Console_Render_Manager_NCurses.hpp
     cli_cpp/cli/A_Console_Render_State.hpp
     cli_cpp/cli/CLIConnectionType.hpp
     cli_cpp/cli/cmd/A_CLI_Command.hpp
     cli_cpp/cli/cmd/A_CLI_Command_Argument.hpp
     cli_cpp/cli/cmd/A_CLI_Command_Parser.hpp
     cli_cpp/cli/cmd/A_CLI_Command_Parser_Factory.hpp
     cli_cpp/cli/cmd/A_CLI_Command_Result.hpp
     cli_cpp/cli/cmd/A_CLI_Parser_Command.hpp
     cli_cpp/cli/cmd/CLICommandArgumentType.cpp
     cli_cpp/cli/cmd/CLICommandArgumentType.hpp
     cli_cpp/cli/cmd/CLICommandParseStatus.hpp
     cli_cpp/thirdparty/ncurses/An_NCurses_Table.hpp
     cli_cpp/thirdparty/ncurses/NCurses_Utilities.hpp
     cli_cpp/thirdparty/pugixml.hpp
     cli_cpp/thirdparty/pugixml/src/pugixml.hpp
     cli_cpp/thirdparty/pugixml/src/pugiconfig.hpp
     cli_cpp/utils/ANSI_Utilities.hpp
     cli_cpp/utils/Log_Utilities.hpp
     cli_cpp/utils/String_Utilities.hpp
     cli_cpp/utils/System_Utilities.hpp
)


#--------------------------------#
#-      Build the Library       -#
#--------------------------------#
add_library( cli_cpp SHARED
             ${CLI_CPP_HEADERS}
             ${CLI_CPP_SOURCES}
)


#------------------------------------#
#-      Link Other Libraries        -#
#------------------------------------#
target_link_libraries( cli_cpp 
                       ${Boost_LIBRARIES}
                       ${CURSES_LIBRARIES} 
                       pthread )


