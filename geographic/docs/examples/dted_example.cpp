#include <iostream>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){


    cout << "usage: ./dted_example  <DTED Name>" << endl;
    cout << " ex:   ./dted_example  ../../data/n37.dt2" << endl;
    if( argc < 2 )
        return 0;

    GEO::GeoImage geoimg( argv[1], true );

    Mat img = geoimg.get_image();
    
    int x = img.cols-1;
    int y = img.rows-1;
    cout << "Elevation of four corners" << endl;
    cout << "NW: " << img.at<short>(0,0) << endl;
    cout << "SW: " << img.at<short>(y,0) << endl;
    cout << "NE: " << img.at<short>(0,x) << endl;
    cout << "SE: " << img.at<short>(y,x) << endl;


    return 0;
}
