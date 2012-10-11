#include <GeoImage.h>

#include <opencv2/core/core.hpp>

#include <iostream>

using namespace cv;
using namespace std;


int main( int argc, char** argv ){


    //create the geoimage
    GEO::GeoImage  gimg( argv[1], true);

    //pull the position of the sensor 
    Mat sensor_position = GEO::GS2::getSensorPosition( gimg );
    
    cout << "Focal Length: " << GEO::GS2::getFocalLength( gimg ) << endl;

    return 0;
}
