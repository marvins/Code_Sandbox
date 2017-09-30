#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    9/29/2017
#  
#    Purpose:  Construct the Sample-GUI Application
#


#---------------------------------#
#-      Define the Project       -#
#---------------------------------#
project(Sample-GUI)

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib )


#--------------------------------#
#-      Define Library List     -#
#--------------------------------#
set( PROJECT_EXTRA_LIBS )

#----------------------------#
#-      Configure GDAL      -#
#----------------------------#
#find_package( GDAL REQUIRED )
#include_directories(${GDAL_INCLUDE_DIR})
#message("-- ${Blue}GDAL Includes:${Green}${GDAL_INCLUDE_DIR}${ColorReset}")


#---------------------------#
#-      Configure Qt5      -#
#---------------------------#
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


# Find the QtWidgets library
find_package(Qt5WebEngine REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)
find_package(Qt5WebSockets REQUIRED )
find_package(Qt5Widgets REQUIRED)



# Add the include path
#  Note:  the find does this during the add_library, but causes issues when I try building
#         objects.
include_directories(${Qt5WebEngine_INCLUDE_DIRS})
include_directories(${Qt5WebEngineWidgets_INCLUDE_DIRS})
include_directories(${Qt5WebSockets_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})



#------------------------------------------#
#-         Define the Main Driver         -#
#------------------------------------------#
set( SAMPLE_GUI_MAIN
    main.cpp
	Main_Window.cpp
	Main_Window.hpp
)


#------------------------------------#
#-      Construct and Link GUI      -#
#------------------------------------#
add_executable( sample-gui
        ${SAMPLE_GUI_MAIN}
)


#   Link the libraries
target_link_libraries( sample-gui
        Qt5::WebEngine
        Qt5::WebEngineWidgets
        Qt5::WebSockets
        Qt5::Widgets
)

