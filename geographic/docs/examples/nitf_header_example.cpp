#include <GeoImage>

#include <iostream>

using namespace std;

void main( int argc, char* argv[] ){


    //load NITF Images 
    GEO::GeoImage img01( argv[1], false );

    //load header data
    GEO::GeoHeader_Info* header_data = img01.get_header_data();
    
    //output relevant header data
    cout << "Relevant header data for image: " << header_data.get_image_filename() << endl;
   
    // get the entire

    // call on random elements using the base class
    

    return 0;
}
