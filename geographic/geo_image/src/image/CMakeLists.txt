
set( IMAGE_SOURCES 
    "src/image/GeoImage.cpp" 
    "src/image/GeoHeader.cpp"
    PARENT_SCOPE )


install (FILES GeoImage.hpp  
               GeoHeader.hpp
         DESTINATION 
               "include/geoimage/image" )

