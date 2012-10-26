


set( IO_SOURCES "src/io/GDALLoader.cpp" "src/io/GeoLoader.cpp" PARENT_SCOPE )

install (FILES GeoLoader.hpp  DESTINATION "include/geoimage/io" )
install (FILES GDALLoader.hpp DESTINATION "include/geoimage/io" )

