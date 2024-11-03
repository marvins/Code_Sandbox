#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    2/15/2018
#


cmake_minimum_required( VERSION 3.0 FATAL_ERROR )


#   Set the project name
PROJECT( Simple-Server-Demo )

#  Set C++ 11
set( CMAKE_CXX_STANDARD 11 )

#  Find libVNC Server
find_package( LibVncServer )
message( " - libVNC-Server Found: ${LIBVNCSERVER_FOUND}")
message( " - libVNC-Server Libs : ${LIBVNCSERVER_LIBRARIES}" )

#---------------------------#
#-       Find OpenCV       -#
#---------------------------#
find_package(OpenCV REQUIRED)


#----------------------------#
#-       Find Boost         -#
#----------------------------#
#SET( Boost_USE_STATIC_LIBS ON )
Find_Package(Boost 
                COMPONENTS
                    system
                    filesystem
                REQUIRED
)
include_directories( ${Boost_INCLUDE_DIRS} )
message(" - Boost_LIBRARIES    : ${Boost_LIBRARIES}")
message(" - Boost_INCLUDE_DIRS : ${Boost_INCLUDE_DIRS}")
message(" - Boost_LIBRARY_DIRS : ${Boost_LIBRARY_DIRS}")


set( DEMO_SOURCES
        simple-server.cpp
        Options.cpp
        Options.hpp
        Server_Context.cpp
        Server_Context.hpp
        ../common/Parser.cpp
        ../common/Parser.hpp
)



add_executable( simple-server
                ${DEMO_SOURCES} )

target_link_libraries( simple-server
                        ${LIBVNCSERVER_LIBRARIES}
                        ${Boost_LIBRARIES}
                        ${OpenCV_LIBRARIES})



