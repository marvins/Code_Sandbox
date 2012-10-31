add_subdirectory( core        )
    
# Add the base header file to the project
install (FILES "${PROJECT_SOURCE_DIR}/src/GeoImage.hpp"  DESTINATION "include" )

include_directories( "${PROJECT_SOURCE_DIR}/src/core"       )


############################################################
#   Check whether or not we are using the IO Module
if( BUILD_DEM_MODULE )
    add_subdirectory( dem )
    include_directories( "${PROJECT_SOURCE_DIR}/src/dem"  )
endif( BUILD_DEM_MODULE )


############################################################
#   Check whether or not we are using the IO Module
if( BUILD_IO_MODULE )
    add_subdirectory( io          )
    include_directories( "${PROJECT_SOURCE_DIR}/src/io"         )
endif( BUILD_IO_MODULE )



###########################################################
#   Check whether or not we are using the extensions Module
if( BUILD_EXTENSION_MODULE )
    add_subdirectory( extensions  )
    include_directories( "${PROJECT_SOURCE_DIR}/src/extensions" )
endif( BUILD_EXTENSION_MODULE )


############################################################
#   Check whether or not we are using the Utility Module
if( BUILD_UTILITY_MODULE )
    add_subdirectory( utilities   )
    include_directories( "${PROJECT_SOURCE_DIR}/src/utilities"  )
endif( BUILD_UTILITY_MODULE )


############################################################
#   Check whether or not we are using the IMAGE Module
if( BUILD_IMAGE_MODULE )
    add_subdirectory( image    )
    include_directories( "${PROJECT_SOURCE_DIR}/src/image"  )
endif( BUILD_IMAGE_MODULE )


############################################################
#   Check whether or not we are using the IO Module
if( BUILD_COORDINATE_MODULE )
    add_subdirectory( coordinates )
    include_directories( "${PROJECT_SOURCE_DIR}/src/coordinates"  )
endif( BUILD_COORDINATE_MODULE )




set( GEO_SOURCES ${GEO_SOURCES} ${DEM_SOURCES}  ${COORDINATE_SOURCES} ${EXTENSION_SOURCES} ${IMAGE_SOURCES}  ${IO_SOURCES}  ${UTILITY_SOURCES}    PARENT_SCOPE )


