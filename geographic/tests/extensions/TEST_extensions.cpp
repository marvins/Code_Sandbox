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

namespace G2 = GEO::GS2;

/** GeoHeader_core */
int TEST_TACID_core(     string& note );
int TEST_TACID_toString( string& note );
int TEST_TACID_isValidTACID( string& note );

void TEST_extensions_module(){
    
    print_module_header("GeoImage extensions");
    
    int result;
    string note;

    /**   GEOHEADER  */
    result = TEST_TACID_core( note );
    print_test_results( "TACID   core", result, note );

    result = TEST_TACID_toString( note );
    print_test_results( "TACID   toString", result, note );

    result = TEST_TACID_isValidTACID( note );
    print_test_results( "TACID   isValidTACID", result, note );

    print_module_footer("GeoImage extensions");

}


/** Run the unit test for the Core GeoHeader Module */
int TEST_TACID_core(     string& note ){
    
    G2::TACID tacid("24FEB129Z0200700ZXGEO000GS0000004F482007.ntf");
    
    note = "Successful Operation";
    return true;
}

int TEST_TACID_toString( string& note ){

    string input = "24FEB129Z0200700ZXGEO000GS0000004F482007.ntf";
        
    note = "unmodified TACID output from toString did not equal input";
    G2::TACID tacid(input);
    
    if( tacid.toString() != input ){ return false; }

    note = "Successful Operation";
    return true;
}

int TEST_TACID_isValidTACID( string& note ){

    /**  Perfect  */
    string input01 = "24FEB129Z0200700ZXGEO000GS0000004F482007.ntf";
    
    /** Bad length */
    string input02 = "24FEEB129Z0200700ZXGEO000GS0000004F482007.ntf";
    
    /** Bad length */
    string input03 = "24FB129Z0200700ZXGEO000GS0000004F482007.ntf";
    
    /** Bad month */
    string input04 = "24FAB129Z0200700ZXGEO000GS0000004F482007.ntf";
    
    note = "TACID should have been valid";
    if( G2::TACID::isValidTACID( input01 ) == false ){ return false; }
    note = "TACID should have been invalid";
    if( G2::TACID::isValidTACID( input02 ) == true ){ return false; }
    if( G2::TACID::isValidTACID( input03 ) == true ){ return false; }
    if( G2::TACID::isValidTACID( input04 ) == true ){ return false; }


    note = "Successful Operation";
    return true;
}

