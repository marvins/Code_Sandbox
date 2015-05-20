#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    5/20/2015
#
#    Purpose:  Constructs the CLI-CPP Sample App Project
#


#   Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall" )


#----------------------------------#
#-    Define our demo sources     -#
#----------------------------------#
set( CLI_APP_SOURCES
     main.cpp
)

#----------------------------------#
#-    Define our demo headers     -#
#----------------------------------#
set( CLI_APP_HEADERS

)


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
                       ${CLI_APP_SOURCES}
                       ${CLI_APP_HEADERS}
                     )


