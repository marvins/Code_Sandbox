#include <iostream>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){


    cout << "usage: ./hello_geoworld   <Image Name>" << endl;
    if( argc < 2 )
        return 0;
    
    //create a GeoImage using the input filename
    GEO::GeoImage geoimg( argv[1], true );
    
    geoimg.modify_header_metadata( "NITF_ISORCE", "what is going on?", 1); 

    geoimg.write_image();

    return 0;
}
