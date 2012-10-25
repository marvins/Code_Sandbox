# Set our required version
cmake_minimum_required (VERSION 2.6)

# Create the version number
project (GeoImage)


# Set the version number
set (GEOIMAGE_VERSION_MAJOR 1)
set (GEOIMAGE_VERSION_MINOR 5)


#Include the source directory
include_directories( "${PROJECT_SOURCE_DIR}/src" )
add_subdirectory( src )


# Add the install targets

