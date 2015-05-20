#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    5/20/2015
#
#    Purpose:  Constructs the CLI-CPP Library Project
#

#   Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall" )

#-------------------------------------#
#-     Find Boost C++ Libraries      -#
#-------------------------------------#
find_package( Boost REQUIRED
                COMPONENTS
                    system
                    filesystem
                    regex
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
     cli/A_CLI_Command_Queue.cpp
     cli/A_CLI_Configuration_File_Parser.cpp
     cli/A_CLI_Connection_Handler_Base.cpp
     cli/A_CLI_Connection_Handler_Base_Config.cpp
     cli/A_CLI_Connection_Handler_Local.cpp
     cli/A_CLI_Connection_Handler_Local_Config.cpp
     cli/A_CLI_Connection_Handler_Socket.cpp
     cli/A_CLI_Connection_Handler_Socket_Config.cpp
     cli/A_CLI_Manager.cpp
     cli/A_CLI_Manager_Configuration.cpp
     cli/A_CLI_Manager_Factory.cpp
     cli/A_Command_History_Entry.cpp
     cli/A_Command_Response_Handler_Base.cpp
     cli/A_Console_Render_Manager.cpp
     cli/A_Console_Render_Manager_ASCII.cpp
     cli/A_Console_Render_Manager_NCurses.cpp
     cli/A_Console_Render_State.cpp
     cli/CLIConnectionType.cpp
     cli/cmd/A_CLI_Command.cpp
     cli/cmd/A_CLI_Command_Argument.cpp
     cli/cmd/A_CLI_Command_Parser.cpp
     cli/cmd/A_CLI_Command_Parser_Factory.cpp
     cli/cmd/A_CLI_Command_Result.cpp
     cli/cmd/A_CLI_Parser_Command.cpp
     cli/cmd/CLICommandArgumentType.cpp
     cli/cmd/CLICommandParseStatus.cpp
     thirdparty/ncurses/NCurses_Utilities.cpp
     thirdparty/pugixml/src/pugixml.cpp
     utils/String_Utilities.cpp
)


#-----------------------------------------#
#-      Define the Library Headers       -#
#-----------------------------------------#
set( CLI_CPP_HEADERS
     cli/A_CLI_Command_Queue.hpp
     cli/A_CLI_Configuration_File_Parser.hpp
     cli/A_CLI_Connection_Handler_Base.hpp
     cli/A_CLI_Connection_Handler_Base_Config.hpp
     cli/A_CLI_Connection_Handler_Local.hpp
     cli/A_CLI_Connection_Handler_Local_Config.hpp
     cli/A_CLI_Connection_Handler_Socket.hpp
     cli/A_CLI_Connection_Handler_Socket_Config.hpp
     cli/A_CLI_Manager.hpp
     cli/A_CLI_Manager_Configuration.hpp
     cli/A_CLI_Manager_Factory.hpp
     cli/A_Command_History_Entry.hpp
     cli/A_Command_Response_Handler_Base.hpp
     cli/A_Console_Render_Manager.hpp
     cli/A_Console_Render_Manager_ASCII.hpp
     cli/A_Console_Render_Manager_NCurses.hpp
     cli/A_Console_Render_State.hpp
     cli/CLIConnectionType.hpp
     cli/cmd/A_CLI_Command.hpp
     cli/cmd/A_CLI_Command_Argument.hpp
     cli/cmd/A_CLI_Command_Parser.hpp
     cli/cmd/A_CLI_Command_Parser_Factory.hpp
     cli/cmd/A_CLI_Command_Result.hpp
     cli/cmd/A_CLI_Parser_Command.hpp
     cli/cmd/CLICommandArgumentType.cpp
     cli/cmd/CLICommandArgumentType.hpp
     cli/cmd/CLICommandParseStatus.hpp
     thirdparty/ncurses/NCurses_Utilities.hpp
     thirdparty/pugixml.hpp
     thirdparty/pugixml/src/pugixml.hpp
     thirdparty/pugixml/src/pugiconfig.hpp
     utils/ANSI_Utilities.hpp
     utils/String_Utilities.hpp
)


#--------------------------------#
#-      Build the Library       -#
#--------------------------------#
add_library( cli_cpp
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


