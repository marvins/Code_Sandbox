#include "TACID.hpp"

#include "IO.hpp"

int TACID::scene_number( string const& filename, bool& isValid ){

    //set to false just in case
    isValid = false;

    //grab basename
    string basename = file_basename( filename ); 
    
    //make sure extension is nitf
    string ext = file_extension( basename );
    if( ext != ".ntf" && ext != ".nitf" )
        return -1;
    
    isValid = true;

    return  str2num<int>(basename.substr(11,5));
}


