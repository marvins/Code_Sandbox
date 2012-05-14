#include <iostream>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){


    cout << "usage: ./hello_geoworld   <Image Name>" << endl;
    if( argc < 2 )
        return 0;

    GeoImage geoimg( argv[1], true );

    Mat img = geoimg.get_image();
    namedWindow("OUTPUT");
    imshow("OUTPUT", img);
    waitKey(0);

    //write the image to file
    string output_filename = "result.ntf";
    GeoHeader_Info* header_info = geoimg.get_header();
    
    GDAL_Data::write( output_filename, img, header_info );


    return 0;
}
