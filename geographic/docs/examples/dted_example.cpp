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
        
        
        // Create a DEM Parameter structure to designate which DEM we wish to use
        // * The first parameter is the type of DEM Data (SRTM or DTED)
        // * The second parameter is the location of the DTED files
        GEO::DEM_Params params(GEO::DTED, "../../data/dted");

        //designate our upper left and bottom right corners
        Point2f ul_big( -119.2500, 37.2500);
        Point2f br_big( -118.7500, 36.7500);
        
        //build a single topo relief map
        GEO::DEM topo_map( ul_big.y, ul_big.x, /** Upper Left  lat, lon */
                           br_big.y, br_big.x, /** Lower Right lat, lon */
                           params);            /** DTED Parameters      */

        //output the DEM tile as a relief map
        Mat relief = topo_map.relief_map();
        
        //output the DEM tile as a DEM map
        Mat data   = topo_map.get_raw();

        //write to file
        imwrite("topo.png", relief);
        

    } catch ( string str ){
        cout << str << endl;
    }
    return 0;
}
