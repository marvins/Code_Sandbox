# - Find LibVNCServer
# Find the LibVNCServer includes and libraries
# This module defines
#  LIBVNCSERVER_INCLUDE_DIR, where to find jpeglib.h and turbojpeg.h, etc.
#  LIBVNCSERVER_LIBRARIES, the libraries needed to use libjpeg-turbo.
#  LIBVNCSERVER_FOUND, If false, do not try to use libjpeg-turbo.

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH( LIBVNCSERVER_INCLUDE_DIR 
            NAMES
                "rfb/rfb.h"
            PATHS
                /usr/include
                /opt/local/include
)

FIND_LIBRARY( LIBVNCSERVER_SERVER_LIBRARY
                    NAMES
                        vncserver
                    PATHS
                        /usr
                        /usr/local
                        /opt/local
                    PATH_SUFFIXES
                        lib
                        lib64)

FIND_LIBRARY( LIBVNCSERVER_CLIENT_LIBRARY
                    NAMES
                        vncclient
                    PATHS
                        /usr
                        /usr/local
                        /opt/local
                    PATH_SUFFIXES
                        lib
                        lib64)


message("LIBVNCSERVER_INCLUDE_DIR    : ${LIBVNCSERVER_INCLUDE_DIR}")
message("LIBVNCSERVER_SERVER_LIBRARY : ${LIBVNCSERVER_SERVER_LIBRARY}")
message("LIBVNCSERVER_CLIENT_LIBRARY : ${LIBVNCSERVER_CLIENT_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set LIBVNCSERVER_FOUND to TRUE if 
# all listed variables are TRUE
#INCLUDE(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibVncClient DEFAULT_MSG LIBVNCSERVER_CLIENT_LIBRARY LIBVNCSERVER_INCLUDE_DIR)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibVncServer DEFAULT_MSG LIBVNCSERVER_SERVER_LIBRARY LIBVNCSERVER_INCLUDE_DIR)

if( LIBVNCSERVER_SERVER_LIBRARY )
    set( LIBVNCSERVER_FOUND TRUE )
endif()
if( LIBVNCSERVER_CLIENT_LIBRARY )
    set( LIBVNCCLIENT_FOUND TRUE )
endif()

IF(LIBVNCCLIENT_FOUND AND LIBVNCSERVER_FOUND)
   SET(LIBVNCSERVER_LIBRARIES ${LIBVNCSERVER_CLIENT_LIBRARY} ${LIBVNCSERVER_SERVER_LIBRARY})
   SET(LIBVNCSERVER_FOUND TRUE)
ENDIF()

MARK_AS_ADVANCED(LIBVNCSERVER_INCLUDE_DIR LIBVNCSERVER_CLIENT_LIBRARY LIBVNCSERVER_SERVER_LIBRARY LIBVNCSERVER_LIBRARIES)

