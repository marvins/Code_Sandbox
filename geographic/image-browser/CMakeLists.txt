#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    3/10/2016
#
#    Purpose: Construct the Image-Browser Project
#


#  Set the CMake Version
cmake_minimum_required( FATAL_ERROR VERSION 2.8 )

#  Set the Project Name
project( ImageBrowser )



#-------------------------------------------#
#  Configure Qt5

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebEngine REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)
find_package(Qt5WebSockets REQUIRED )


#----------------------------------#
#-         Configure GDAL         -#
#----------------------------------#
FIND_PACKAGE( GDAL REQUIRED )
include_directories( ${GDAL_INCLUDE_DIR} )


#----------------------------------#
#-         Configure Boost        -#
#----------------------------------#
Find_Package(Boost
                COMPONENTS
                    date_time
                    regex
                        system
                        filesystem
                    REQUIRED
)
include_directories( ${Boost_INCLUDE_DIRS} )
message("Boost Libraries: ${Boost_LIBRARIES}")
message("Boost Includes : ${Boost_INCLUDE_DIRS}")



#---------------------------------#
#-      Define our Sources       -#
#---------------------------------#
set( IMAGE_BROWSER_SOURCES
     src/imagebrowser.cpp
     src/core/DataContainer.cpp
     src/core/FilesystemUtilities.cpp
     src/core/GDALLoader.cpp
     src/core/Image.cpp
     src/core/ImageDatabase.cpp
     src/core/Log_Utilities.cpp
     src/core/MessagingService.cpp
     src/core/Overlay.cpp
     src/core/Point.cpp
     src/core/Rect.cpp
     src/core/StringUtilities.cpp
     src/gui/AssetPane.cpp
     src/gui/BrowserPage.cpp
     src/gui/BrowserPane.cpp
     src/gui/IndexingProgressDialog.cpp
     src/gui/IndexingWorker.cpp
     src/gui/MainWindow.cpp
     src/gui/PreferenceDialog.cpp
)



set( IMAGE_BROWSER_HEADERS
     src/core/DataContainer.hpp
     src/core/Enumerations.hpp
     src/core/FilesystemUtilities.hpp
     src/core/Image.hpp
     src/core/ImageDatabase.hpp
	 src/core/GDALLoader.hpp
     src/core/Log_Utilities.hpp
     src/core/MessagingService.hpp
     src/core/Overlay.hpp
	 src/core/Point.hpp
     src/core/Rect.hpp
	 src/core/StringUtilities.hpp
     src/gui/AssetPane.hpp
     src/gui/BrowserPage.hpp
     src/gui/BrowserPane.hpp
     src/gui/IndexingProgressDialog.hpp
     src/gui/IndexingWorker.hpp
     src/gui/MainWindow.hpp
     src/gui/PreferenceDialog.hpp
)


add_executable( image_browser
                ${IMAGE_BROWSER_SOURCES}
                ${IMAGE_BROWSER_HEADERS} )

# Use the Widgets module from Qt 5.
target_link_libraries( image_browser
                       Qt5::Widgets
                       Qt5::WebEngine
                       Qt5::WebEngineWidgets
                       Qt5::WebSockets
                       ${GDAL_LIBRARY}
                       ${Boost_LIBRARIES}
)
