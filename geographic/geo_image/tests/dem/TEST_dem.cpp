#include "TEST_dem.hpp"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <string> 

#include <Logger.hpp>

#include <GeoImage.hpp>

using namespace cv;
using namespace std;



/** 
 * Beginning of the structures module test
 *
 * Modules covered
 * - PixelType
*/
int TEST_dem_constructors( string& note );


/** DEM Module Unit Test Suite */
void TEST_dem_module(){
   
    int result;
    string note;

    /** Print Module Header */
    print_module_header("DEM");

    /** DEM Constructor     */
    result = TEST_dem_constructors( note );
    print_test_results( "DEM    Constructors", result, note );

    /** DEM Footer          */
    print_module_footer("DEM");   

}


/** DEM Constructors        */
int TEST_dem_constructors( string& note ){
    
    /**
     *  Notes for development of unit tests
     *
     *  for w119/n037.dt2
     *      max point is 4325  at  x= 2680, y= 1318
     *      min point is 1166  at  x= 2814, y= 3600
     *
     *  for w119/n038.dt2
     *      max point is 3418  at  x= 2680, y= 1318
     *      min point is 1166  at  x= 2814, y= 3600
    */
    //create DEM object
    double tl_lat =   39.1;
    double tl_lon = -118.91;
    double br_lat =   38.00;
    double br_lon = -118.01;
    
    Mat timg01 = Mat::ones( 100, 100, CV_16SC1 );

    // create a series of DEM objects
    GEO::DEM dem01;
    GEO::DEM dem02( Point2f(-500, -500), Point2f(500,500), timg01 );
    
    // check for the tile size
    note = "Tile not correct size.";
    if( dem01.get_tile().cols != 0   || dem01.get_tile().rows != 0   ) return false;
    if( dem02.get_tile().cols != 100 || dem02.get_tile().rows != 100 ) return false;
    

    //GEO::DEM dem_01( tl_lat, tl_lon, br_lat, br_lon, GEO::DEM_Params( GEO::DTED, "data/dted")); 
    
    //Mat tile01 = dem_01.get_raw();
    
    //double lat01, lon01;
    //double elev = dem_01.max_elevation( lat01, lon01 );
    
    note = "Successful Operation";
    return false;
}

