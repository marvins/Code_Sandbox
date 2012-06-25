#include <iostream>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){


    cout << "usage: ./hello_geoworld   <Image Name>" << endl;
    if( argc < 2 )
        return 0;

    GEO::GeoImage geoimg( argv[1], true );

    Mat img = geoimg.get_image();

    //write the image to file
    string output_filename = "result.ntf";
    GEO::GeoHeader_Info* header_info = geoimg.get_header();
   
    header_info->set_header_item("NITF_ISORCE","HELLO");
    header_info->set_header_item("NITF_ISREL","WORLD");

    GEO::GDAL_Data::write( output_filename, img, header_info );


    return 0;
}
