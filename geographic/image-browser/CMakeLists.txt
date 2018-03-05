#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    3/10/2016
#
#    Purpose: Construct the Image-Browser Project
#


#  Set the CMake Version
cmake_minimum_required( FATAL_ERROR VERSION 2.8 )

#  Set the Project Name
project( Geo-Data-Manager )

LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

#-------------------------------------------#
#-             Configure Qt5               -#
#-------------------------------------------#

LIST( APPEND CMAKE_MODULE_PATH "$ENV{QT5_ROOT}/lib/cmake")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5OpenGL  REQUIRED)


get_target_property(Qt5Widgets_LIBRARIES Qt5::Widgets LOCATION)
message("Qt5::Widgets: ${Qt5Widgets_LIBRARIES}")

#---------------------------------#
#-      Configure ArcGIS SDK     -#
#---------------------------------#
find_package(ArcGIS_SDK)

message("ArcGIS_SDK Found")
message("   - ArcGIS_SDK_INCLUDE_DIR: ${ArcGIS_SDK_INCLUDE_DIR}")
message("   - ArcGIS_SDK_LIBRARIES:   ${ArcGIS_SDK_LIBRARIES}" )
include_directories(${ArcGIS_SDK_INCLUDE_DIR})


#---------------------------------#
#-      Define our Sources       -#
#---------------------------------#
set( GEO_DATA_MANAGER_SOURCES
        src/geo-data-manager.cpp
        src/asset/Asset_Manager.cpp
        src/core/Geo_Options.cpp
        src/gui/Main_Window.cpp
        src/gui/asset_panel/Asset_Panel.cpp
)



set( GEO_DATA_MANAGER_HEADERS
        src/asset/Asset_Manager.hpp
        src/core/Geo_Options.hpp
        src/gui/Main_Window.hpp
        src/gui/asset_panel/Asset_Panel.hpp
)


add_executable( geo-data-manager
                ${GEO_DATA_MANAGER_SOURCES}
                ${GEO_DATA_MANAGER_HEADERS} )

# Use the Widgets module from Qt 5.
target_link_libraries( geo-data-manager
                       Qt5::Network
                       Qt5::OpenGL
                       Qt5::Widgets
                       ${ArcGIS_SDK_LIBRARIES}
)
