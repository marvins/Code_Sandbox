add_subdirectory( core )
add_subdirectory(  io  )
add_subdirectory( utilities )

# Add the base header file to the project
install (FILES "${PROJECT_SOURCE_DIR}/src/GeoImage.hpp"  DESTINATION "include" )

include_directories( "${PROJECT_SOURCE_DIR}/src/core"      )
include_directories( "${PROJECT_SOURCE_DIR}/src/io"        )
include_directories( "${PROJECT_SOURCE_DIR}/src/utilities" )

set( GEO_SOURCES ${GEO_SOURCES} ${IO_SOURCES}  ${UTILITY_SOURCES}    PARENT_SCOPE )


