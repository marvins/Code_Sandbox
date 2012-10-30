
set( UTILITY_SOURCES 
        "src/utilities/DEM_Utilities.cpp"
        "src/utilities/File_Utilities.cpp"
        "src/utilities/Math_Utilities.cpp"
        "src/utilities/OpenCV_Utilities.cpp"
        "src/utilities/String_Utilities.cpp"
        PARENT_SCOPE )

install (FILES 
                DEM_Utilities.hpp
                File_Utilities.hpp  
                Math_Utilities.hpp
                OpenCV_Utilities.hpp
                String_Utilities.hpp
         DESTINATION 
                "include/geoimage/utilities" )

