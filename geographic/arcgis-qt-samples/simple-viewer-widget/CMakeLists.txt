#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    8/1/2017
#
#    Purpose:  Base-Level CMake Build Script.
#

#  Set Required CMake Version
cmake_minimum_required( VERSION 3.0 )

#  Use C++11
set (CMAKE_CXX_STANDARD 14)


#  Name Project
project(Simple-Viewer-Widget)


#-----------------------------#
#-      Configure Boost      -#
#-----------------------------#
find_package( Boost
                COMPONENTS
                    filesystem
              REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})


#----------------------------#
#-      Configure GDAL      -#
#----------------------------#
find_package( GDAL REQUIRED )
include_directories(${GDAL_INCLUDE_DIR})


#------------------------------#
#-      Configure OpenCV      -#
#------------------------------#
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})


#---------------------------------#
#-      Configure ArcGIS SDK     -#
#---------------------------------#
include_directories(/opt/software/arcgis/runtime_sdk/qt100.1/sdk/include)
find_library( ArcGIS_Runtime_QT_LIB
                EsriRuntimeQt
                PATHS
                    /opt/software/arcgis/runtime_sdk/qt100.1/sdk/linux/x64/lib
)
find_library( ArcGIS_Runtime_RT_CORE_LIB
                runtimecore
                PATHS
                    /opt/software/arcgis/runtime_sdk/qt100.1/sdk/linux/x64/lib
)
find_library( ArcGIS_Runtime_QT_COMMON_LIB
                libEsriCommonQt.so
                PATHS
                    /opt/software/arcgis/runtime_sdk/qt100.1/sdk/linux/x64/lib
)

set( ArcGIS_LIBS 
        ${ArcGIS_Runtime_QT_LIB}
        ${ArcGIS_Runtime_RT_CORE_LIB}
        ${ArcGIS_Runtime_QT_COMMON_LIB}
)                

#---------------------------#
#-      Configure Qt5      -#
#---------------------------#

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngine REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)
find_package(Qt5WebSockets REQUIRED )

list( APPEND GEO_REGISTRATION_LIBRARIES
        Qt5::WebEngine
        Qt5::WebEngineWidgets
        Qt5::WebSockets
        Qt5::Widgets)


set( SIMPLE_VIEWER_SOURCES
    simple-viewer.cpp
    Main_Window.cpp
)

set( SIMPLE_VIEWER_HEADERS
     Main_Window.hpp
)


add_executable( simple-viewer-widget
                ${SIMPLE_VIEWER_SOURCES}
                ${SIMPLE_VIEWER_HEADERS} )

target_link_libraries( simple-viewer-widget
                       pthread
                       ${ArcGIS_LIBS}
                       Qt5::WebEngine
                       Qt5::WebEngineWidgets
                       Qt5::WebSockets
                       Qt5::Widgets
                       ${GDAL_LIBRARY}
                       ${Boost_LIBRARIES}
                       ${OpenCV_LIBS} )

