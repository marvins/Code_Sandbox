#include <GeoImage.h>

using namespace std;

int main( int argc, char* argv[] ){

    //load the appropriate image
    GEO::GeoImage img( argv[1], true);

    //check if the image is a GS2 TACID
    if( GEO::GS2::isTACID( img.get_filename()) == false ){
        cout << "Not valid GS2 Image" << endl;
        return -1;
    }
    
    //create the TACID object
    GEO::GS2::TACID tacid( img.get_filename() );
    
    //modify some tacid information
    //  - note: See GS2 ITT ICD for specification information

    //swap EO / IR flag
    if( tacid.


    //modify image metadata


    //change the image location



    //modify the TACID


    
    //modify the actual image



    //write the image



    return 0;
}

