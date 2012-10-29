
set( IO_SOURCES "src/io/GDALLoader.cpp" "src/io/GeoLoader.cpp" PARENT_SCOPE )

configure_file( 
    "IO_Configuration.hpp.in"
    "${PROJECT_SOURCE_DIR}/src/io/IO_Configuration.hpp"
)

configure_file( 
    "IO_Configuration.hpp.in"
    "${PROJECT_BINARY_DIR}/src/io/IO_Configuration.hpp"
)

install (FILES GeoLoader.hpp  DESTINATION "include/geoimage/io" )
install (FILES GDALLoader.hpp DESTINATION "include/geoimage/io" )
install (FILES IO_Configuration.hpp DESTINATION "include/geoimage/io" )

