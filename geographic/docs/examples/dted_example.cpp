#include <iostream>

#include <GeoImage.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

    try{

        //find the largest coordinate in the DTED tile
        double elevation, lat, lon;
        GEO::GeoImage  gimg("../../data/dted/w119/n036.dt2", true);

        //make sure that the returned image is the expected datatype
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

        // lets compare this with the expected value of the high point
        lat = lat/((double)img.rows-1);
        lon = 1-lon/((double)img.cols-1);
        cout << "actual coordinate location is " << lon << ", " << lat << endl;

        //query the point independent of the dted file
        Point2f pnt( -118.2922, 36.57857 );
        GEO::DEM_Params params(GEO::DTED, "../../data/dted");

        elevation = GEO::DEM( pnt, params ).get_elevation( );
        cout << "Elevation: " << elevation << endl;

        //lets pull a dted tile which is an odd size
        Point2f ul( -118.4  , 36.7  ); 
        Point2f br( -118.15 , 36.45 );
        
        GEO::DEM topo_map( ul.y, ul.x, br.y, br.x, params );

        Mat color_map = topo_map.relief_map();

        //recover the pixel location of Mount Whitney
        Point whitney = topo_map.get_pixel_coordinate( pnt );
        
        circle( color_map, whitney, 8, Scalar(0, 0, 255), 2);

        imwrite( "new.png", color_map);

    } catch ( string str ){
        cout << str << endl;
    }
    return 0;
}
