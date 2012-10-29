# Set our required version
cmake_minimum_required (VERSION 2.6)

# Create the version number
project (GeoImage)

# Set the version number
set (GEOIMAGE_VERSION_MAJOR 1)
set (GEOIMAGE_VERSION_MINOR 5)

# Instanciate the library variable
set( LIBRARY_NAME GeoImage )

set( GEO_SOURCES )



####################################################
#          EXTENSION Requirements and Build        #
####################################################
option (BUILD_EXTENSION_MODULE "Build the optional, add-on code"  ON )

####################################################
#           IMAGE Requirements and Build           #
####################################################
option (BUILD_IMAGE_MODULE "Build IMAGE Module (Requires OpenCV)" ON )
set( USE_OPENCV 0 )
if( BUILD_IMAGE_MODULE )
    
    # Pull in OpenCV
    set( USE_OPENCV 1 )
    FIND_PACKAGE( OpenCV REQUIRED )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    #set( LIBS  ${LIBS} ${OpenCV_LIBS} )
    
endif( BUILD_IMAGE_MODULE )

####################################################
#             IO Requirements and Build            #
####################################################
option (BUILD_IO_MODULE "Build IO Module (Required GDAL)" ON )
if( BUILD_IO_MODULE )

    #Pull in GDAL
    find_package( GDAL REQUIRED )
    include_directories( ${GDAL_INCLUDE_DIR} )
    set( LIBS ${LIBS} ${GDAL_LIBRARY} )

endif( BUILD_IO_MODULE )


#####################################################
#             Utilities Requirements                #
#####################################################
option (BUILD_UTILITY_MODULE "Build Utility Module (Requires Boost Filesystem, System, Algorithm)"  ON )
if( BUILD_UTILITY_MODULE )

    #Pull in Boost
    find_package( Boost REQUIRED COMPONENTS system filesystem )
    include_directories( ${Boost_INCLUDE_DIRS} )
    set( LIBS ${LIBS} ${Boost_LIBRARIES} )

endif( BUILD_UTILITY_MODULE )


#Include the source directory
include_directories( "${PROJECT_SOURCE_DIR}/src" )
add_subdirectory( src )



# Create the GeoImage Library
add_library( ${LIBRARY_NAME} SHARED ${GEO_SOURCES} ${LIBS} )
target_link_libraries( ${LIBRARY_NAME} ${OpenCV_LIBS} )

########################################################
#              Geo_Meta_Parser Application             #
########################################################
option (BUILD_GEO_META_PARSER  "Build the geo_meta_parser" ON )
if( BUILD_GEO_META_PARSER )

    # Create the geo_meta_parser binary
    add_executable( geo_meta_parser
                    ${GEO_SOURCES} 
                    ${PROJECT_SOURCE_DIR}/src/tools/geo_meta_parser.cpp
                    ${PROJECT_SOURCE_DIR}/src/tools/geo_meta_parser/io.cpp
                    ${PROJECT_SOURCE_DIR}/src/tools/geo_meta_parser/utilities.cpp
                )
    target_link_libraries( geo_meta_parser   ${LIBS} ${OpenCV_LIBS}  )
    install (FILES "${PROJECT_BINARY_DIR}/geo_meta_parser"       DESTINATION bin )

endif( BUILD_GEO_META_PARSER )


########################################
#        Unit Test Application         #
########################################
add_custom_target(  check
                    COMMAND make -f "${PROJECT_SOURCE_DIR}/tests/Makefile" 
                    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                    )


install (FILES "${PROJECT_BINARY_DIR}/lib${LIBRARY_NAME}.so" DESTINATION lib ) 

set ( CMAKE_INSTALL_PREFIX "/opt/local" )
