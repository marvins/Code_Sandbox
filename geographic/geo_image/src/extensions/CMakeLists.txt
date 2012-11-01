


set( EXTENSION_SOURCES  
    "src/extensions/GS_ICD_Reference.hpp"
    "src/extensions/TACID.cpp"  
    "src/extensions/GS2_Header.cpp" 
    PARENT_SCOPE )

install (FILES GS_ICD_Reference.hpp DESTINATION "include/geoimage/extensions" )
install (FILES TACID.hpp        DESTINATION "include/geoimage/extensions"  )
install (FILES GS2_Header.hpp  DESTINATION "include/geoimage/extensions"  )

