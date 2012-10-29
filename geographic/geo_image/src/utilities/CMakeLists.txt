
set( UTILITY_SOURCES 
        "src/utilities/File_Utilities.cpp"
        "src/utilities/OpenCV_Utilities.cpp"
        "src/utilities/String_Utilities.cpp"
        PARENT_SCOPE )

install (FILES 
                File_Utilities.hpp  
                OpenCV_Utilities.hpp
                String_Utilities.hpp
         DESTINATION 
                "include/geoimage/utilities" )

