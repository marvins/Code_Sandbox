#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    9/3/2015
#
#    Purpose:  Build the image viewer project.
#

#   Set the minimum cmake version
cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

#   Allow C++11x
set(CMAKE_CXX_FLAGS "-std=c++0x")


#--------------------------------#
#-     Find GLFW 3 Library      -#
#--------------------------------#
find_package( glfw3 REQUIRED )
if( EXISTS ${GLFW3_LIBRARY} )
    
    LIST(APPEND GLFW_LIBRARY_LIST ${GLFW3_LIBRARY})

    if(APPLE)
        if( EXISTS "/opt/local/include/GLFW" )
            include_directories("/opt/local/include/GLFW")
        endif()
        MESSAGE("Finding Mac GLFW Libraries")
    elseif(UNIX)
        
        #  Find X11
        FIND_PACKAGE(X11 REQUIRED)
        LIST(APPEND GLFW_LIBRARY_LIST ${X11_LIBRARIES})
    endif()
endif()


#-------------------------------------------#
#-  Find OpenGL Extension Wrangler (GLEW)  -#
#-------------------------------------------#
find_package( GLEW REQUIRED )
include_directories( ${GLEW_INCLUDE_DIR})

#------------------------------------#
#-  Find OpenGL Libraries (Drivers) -#
#------------------------------------#
find_package( OpenGL REQUIRED )


#----------------------------#
#-    Find PugiXML Files    -#
#----------------------------#
set( PUGIXML_SOURCES
     src/thirdparty/pugixml/src/pugixml.hpp
     src/thirdparty/pugixml/src/pugiconfig.hpp
     src/thirdparty/pugixml/src/pugixml.cpp
)
include_directories(src/thirdparty/pugixml/src)


#  Files
set( VIEWER_FILES
     src/main.cpp
     src/OpenGL_Utilities.cpp
     src/OpenGL_Utilities.hpp
     src/Window_Utilities.cpp
     src/Window_Utilities.hpp
)


#---------------------------------------#
#-       Build the Application         -#
#---------------------------------------#
add_executable( viewer-application
                ${VIEWER_FILES}
                ${PUGIXML_SOURCES}
)


target_link_libraries( viewer-application
                       ${OPENGL_LIBRARIES}
                       ${GLEW_LIBRARY}
                       ${GLFW_LIBRARY_LIST}
)

