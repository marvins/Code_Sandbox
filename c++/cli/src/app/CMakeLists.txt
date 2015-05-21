#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    5/20/2015
#
#    Purpose:  Constructs the CLI-CPP Sample App Project
#


#   Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall" )

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )


#----------------------------------#
#-    Define our demo sources     -#
#----------------------------------#
set( CLI_APP_SOURCES
     main.cpp
     cli/A_Ping_Command_Response_Handler.cpp
     utils/System_Utilities.cpp
)

#----------------------------------#
#-    Define our demo headers     -#
#----------------------------------#
set( CLI_APP_HEADERS
     cli/A_Ping_Command_Response_Handler.hpp
     utils/System_Utilities.hpp
)

#----------------------------------#
#-      Include the library       -#
#----------------------------------#
include_directories(../lib)

#----------------------------------#
#-      Build the Executable      -#
#----------------------------------#
add_executable( cli-network-tester
                ${CLI_APP_SOURCES}
                ${CLI_APP_HEADERS}
)


#-------------------------------------#
#-      Link Required Libraries      -#
#-------------------------------------#
target_link_libraries( cli-network-tester 
                       cli_cpp
                     )


