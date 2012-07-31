#include <iostream>

#include <GeoImage.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

    try{

        //query the point independent of the dted file
        Point2f pnt( -118.2922, 36.57857 );
        GEO::DEM_Params params(GEO::DTED, "../../data/dted");

        double elevation = GEO::DEM( pnt, params ).get_elevation( );
        cout << "Elevation: " << elevation << endl;

        //lets pull a dted tile which is an odd size
        Point2f ul_lower( -118.500 , 36.999 ); 
        Point2f br_lower( -118.0001, 36.75  );
        
        Point2f ul_upper( -118.500 , 37.25  );
        Point2f br_upper( -118.0001, 37.00  );
        
        GEO::DEM topo_map_lower( ul_lower.y, ul_lower.x, br_lower.y, br_lower.x, params );
        GEO::DEM topo_map_upper( ul_upper.y, ul_upper.x, br_upper.y, br_upper.x, params );

        Mat color_map_lower = topo_map_lower.relief_map();
        Mat color_map_upper = topo_map_upper.relief_map();

        //recover the pixel location of Mount Whitney
        //Point whitney = topo_map.get_pixel_coordinate( pnt );
        
        //circle( color_map, whitney, 8, Scalar(0, 0, 255), 2);

        imwrite( "upper.png", color_map_upper);
        imwrite( "lower.png", color_map_lower);

    } catch ( string str ){
        cout << str << endl;
    }
    return 0;
}
