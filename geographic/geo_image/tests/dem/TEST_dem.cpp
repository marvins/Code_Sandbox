#include "TEST_dem.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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
int TEST_dem_elevation_accuracy( string& note );

/** DEM Module Unit Test Suite */
void TEST_dem_module(){
   
    int result;
    string note;

    /** Print Module Header */
    print_module_header("DEM");

    /** DEM Constructor     */
    result = TEST_dem_constructors( note );
    print_test_results( "DEM    Constructors", result, note );
    
    /** DEM Elevation Accuracy */
    result = TEST_dem_elevation_accuracy( note );
    print_test_results( "DEM    Elevation Accuracy", result, note );

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
    GEO::DEM dem02( Point2f( -500, -500), Point2f( 500, 500), timg01 );
    GEO::DEM dem03( Point2f( -500, -500), Point2f( 500, 500), imread("tests/dem01.png",0));
    GEO::DEM dem04( Point2f( -120,   38), Point2f(-119, 39 ), GEO::DEM_Params( GEO::DTED, "data/dted" )); 

    // check for the tile size
    note = "Tile not correct size.";
    if( dem01.get_tile().cols != 0    || dem01.get_tile().rows != 0   ) return false;
    if( dem02.get_tile().cols != 100  || dem02.get_tile().rows != 100 ) return false;
    if( dem03.get_tile().cols != 1000 || dem03.get_tile().rows != 1000 ) return false;
    if( dem04.get_tile().cols != 3601 || dem04.get_tile().rows != 3601 ) return false;

    //check corners
    Point2f ne(  500,  500);
    Point2f nw( -500,  500);
    Point2f se(  500, -500);
    Point2f sw( -500, -500);
    note = "Corner Locations Failed";
    if( norm(dem01.ne() - Point2f(0,0) ) > 0.001 ) return false;
    if( norm(dem02.ne() - ne ) > 0.001 ) return false;
    if( norm(dem02.nw() - nw ) > 0.001 ) return false;
    if( norm(dem02.se() - se ) > 0.001 ) return false;
    if( norm(dem02.sw() - sw ) > 0.001 ) return false;
    if( norm(dem04.sw() - Point2f(-120,38)) > 0.0001 ) return false;
    if( norm(dem04.ne() - Point2f(-119,39)) > 0.0001 ) return false;

    note = "Successful Operation";
    return true;
}


int TEST_dem_elevation_accuracy( string& note ){

    // Create the DEM objects
    GEO::DEM dem04( Point2f( -120,   38), Point2f(-119, 39 ), GEO::DEM_Params( GEO::DTED, "data/dted" )); 
    
    // make sure the max elevation is expected
    note = "Max Elevation is incorrect";
    if( fabs( dem04.max_elevation() - 3783 ) > 0.001 ) return false; 
    
    Point2f max04;
    if( fabs( dem04.max_elevation(max04) - 3783 ) > 0.001 ) return false; 
    cout << max04 << endl;

    note = "Successful Operation";
    return true;
}


