#include "TEST_extensions.h"

//GeoImage includes
#include <GeoImage.h> 

#include <fstream>
#include <iostream>
#include <string> 

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <gdal_priv.h>
#include <cpl_conv.h>

#include <Logger.h>
#include <string_utilities.h>

using namespace cv;
using namespace std;

/** GeoHeader_core */
int TEST_TACID_core(     string& note );

void TEST_extensions_module(){
    
    print_module_header("GeoImage extensions");
    
    int result;
    string note;

    /**   GEOHEADER  */
    result = TEST_TACID_core( note );
    print_test_results( "TACID   core", result, note );
    

    print_module_footer("GeoImage extensions");

}


/** Run the unit test for the Core GeoHeader Module */
int TEST_TACID_core(     string& note ){

    note = "Not Implemented";
    return false;
}


