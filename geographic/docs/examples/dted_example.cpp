#include <iostream>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){
    

    //find the largest coordinate in the DTED tile
    double elevation, lat, lon;
    GEO::GeoImage  gimg("../../data/dted/w119/n036.dt2", true);
    
    Mat img = gimg.get_image();

    if( img.type() != CV_16SC1 ) throw string("Not CV_16SC1");

    elevation = 0;
    for( int i=0; i<img.cols; i++)
    for( int j=0; j<img.rows; j++)
        if( elevation < img.at<short>(j,i) ){
            elevation = img.at<short>(j,i);
            lat = img.rows - j;
            lon = i;
        }

    cout << "Max elevation is " << elevation << "  -->  " << lat << ", " << lon << endl;
    
    lat = lat/((double)img.rows-1);
    lon = 1-lon/((double)img.cols-1);
    cout << "actual coordinate location is " << lat << ", " << lon << endl;

    //query the point 
    Point2f pnt( -118.2922, 36.57857 );
    GEO::DEM_Params params(GEO::DTED, "../../data/dted");

    elevation = GEO::DEM( pnt, params ).get_elevation( );
    cout << "Elevation: " << elevation << endl;

    return 0;
}
