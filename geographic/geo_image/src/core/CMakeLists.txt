
configure_file( 
    "GeoImage_Version.hpp.in"
    "${PROJECT_BINARY_DIR}/src/core/GeoImage_Version.hpp"
)

install (FILES "${PROJECT_BINARY_DIR}/src/core/GeoImage_Version.hpp"  DESTINATION "include/GeoImage/core" )

