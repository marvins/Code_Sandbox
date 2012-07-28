#include "TEST_dem.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <string> 

#include <Logger.h>

#include <GeoImage.h>

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
   
    cout << "start of operation" << endl;
    GEO::DEM dem_01( tl_lat, tl_lon, br_lat, br_lon, GEO::DEM_Params( GEO::DTED, "data/dted")); 
    
    cout << "pulling raw" << endl;
    Mat tile01 = dem_01.get_raw();
    
    double lat01, lon01;
    double elev = dem_01.max_elevation( lat01, lon01 );
    

    
    note = "Operation failed";
    return false;
}

