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

void TEST_dem_module(){
   
    int result;
    string note;

    print_module_header("DEM");

    result = TEST_dem_constructors( note );
    print_test_results( "DEM    Constructors", result, note );


    print_module_footer("DEM");   

}

int TEST_dem_constructors( string& note ){
    
    //create DEM object
    double tl_lat =   39.1;
    double tl_lon = -118.1;
    double br_lat =   37.2;
    double br_lon = -118.01;

    GEO::DEM dem_01( tl_lat, tl_lon, br_lat, br_lon, GEO::DEM_Params( GEO::DTED, "data/dted")); 

    Mat tile01 = dem_01.get_raw();

    note = "Operation failed";
    return false;
}

