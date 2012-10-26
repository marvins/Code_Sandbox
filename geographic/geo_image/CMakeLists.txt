# Set our required version
cmake_minimum_required (VERSION 2.6)

# Create the version number
project (GeoImage)

# Set the version number
set (GEOIMAGE_VERSION_MAJOR 1)
set (GEOIMAGE_VERSION_MINOR 5)


# Instanciate the library variable
set( LIBRARY_NAME GeoImage )

set( GEO_SOURCES PARENT_SCOPE )


#Include the source directory
include_directories( "${PROJECT_SOURCE_DIR}/src" )
add_subdirectory( src )


#Pull in GDAL
find_package( GDAL REQUIRED )
include_directories( ${GDAL_INCLUDE_DIR} )
set( LIBS ${LIBS} ${GDAL_LIBRARY} )


#Pull in Boost
find_package( Boost REQUIRED COMPONENTS system filesystem )
include_directories( ${Boost_INCLUDE_DIRS} )
set( LIBS ${LIBS} ${Boost_LIBRARIES} )


# Add the install targets
add_library( "${LIBRARY_NAME}" SHARED ${GEO_SOURCES} ${LIBS} )


install (FILES "${PROJECT_BINARY_DIR}/lib${LIBRARY_NAME}.so" DESTINATION lib ) 
