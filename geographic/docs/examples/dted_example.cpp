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


        //lets pull a dted tile set which requires 4 tiles
        Point2f ul_nw( -119.2500, 37.2500);
        Point2f br_nw( -119.0001, 37.0000);
        
        Point2f ul_ne( -119.0000, 37.2500);
        Point2f br_ne( -118.7500, 37.0000);
        
        Point2f ul_se( -119.0000, 36.9999);
        Point2f br_se( -118.7500, 36.7500);
        
        Point2f ul_sw( -119.2500, 36.9999);
        Point2f br_sw( -119.0001, 36.7500);
        

        //build 4 topo maps
        GEO::DEM topo_map_nw( ul_nw.y, ul_nw.x, br_nw.y, br_nw.x, params);
        GEO::DEM topo_map_ne( ul_ne.y, ul_ne.x, br_ne.y, br_ne.x, params);
        GEO::DEM topo_map_sw( ul_sw.y, ul_sw.x, br_sw.y, br_sw.x, params);
        GEO::DEM topo_map_se( ul_se.y, ul_se.x, br_se.y, br_se.x, params);

        Mat color_map_nw = topo_map_nw.relief_map();
        Mat color_map_ne = topo_map_ne.relief_map();
        Mat color_map_sw = topo_map_sw.relief_map();
        Mat color_map_se = topo_map_se.relief_map();

        //write images to file
        imwrite( "nw.png", color_map_nw);
        imwrite( "ne.png", color_map_ne);
        imwrite( "sw.png", color_map_sw);
        imwrite( "se.png", color_map_se);

    } catch ( string str ){
        cout << str << endl;
    }
    return 0;
}
