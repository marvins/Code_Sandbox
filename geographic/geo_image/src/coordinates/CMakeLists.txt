
set( COORDINATE_SOURCES
     "src/coordinates/Datum.cpp"
     "src/coordinates/CoordinateLatLon.cpp"
     "src/coordinates/CoordinateUTM.cpp"
     "src/coordinates/Conversion_Utilities.cpp"
     "src/coordinates/Coordinate_Math.cpp"
     PARENT_SCOPE )

install ( FILES Datum.hpp
                CoordinateLatLon.hpp
                CoordinateUTM.hpp
                Conversion_Utilities.hpp
                Coordinate_Math.hpp
          DESTINATION
                "include/geoimage/coordinates" )

