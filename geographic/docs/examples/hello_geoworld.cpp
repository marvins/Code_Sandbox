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

    return 0;
}
