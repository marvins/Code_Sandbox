#include "TEST_image.hpp"

// add Logging code
#include <Logger.hpp>

// add GeoImage library
#include <GeoImage.hpp>

// add OpenCV Library
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int TEST_GeoImage_constructor( string& note );
int TEST_GeoImage_validate( string& note );
int TEST_GeoImage_get_image( string& note );


void TEST_image_module(){

    /** test image data */
    int result;
    string note;

    print_module_header("GeoImage");
    
    // GeoImage Constructor
    result = TEST_GeoImage_constructor( note );
    print_test_results( "GeoImage    Constructor", result, note );
    
    // GeoImage Validate
    result = TEST_GeoImage_validate( note );
    print_test_results( "GeoImage    Validated Data", result, note );
    
    // GeoImage Get Image
    result = TEST_GeoImage_get_image( note );
    print_test_results( "GeoImage    Get_Image", result, note );


    print_module_footer("GeoImage");

}

int TEST_GeoImage_constructor( string& note ){
    
    
    // create test geoimage
    string fname01 = "tests/test.ntf";
    string fname02 = "tests/tests.ntf";
    GEO::GeoImage geoimage01( fname01, true);
    GEO::GeoImage geoimage02( fname02, true);


    // check the init flag
    note = "GeoImage Constructor init flag is invalid";
    if( geoimage01.get_init() == false )
        return 0;
    if( geoimage02.get_init() == true )
        return 0;
   

    // check the filename
    note = "Image Filename is different from expected.";
    if( fname01 != geoimage01.get_filename() ) return 0;
    if( fname02 != geoimage02.get_filename() ) return 0;
    
    // return data
    note = "Operation Successful";
    return 1;
}


int TEST_GeoImage_validate( string& note ){

    // create test geoimage
    string fname01 = "tests/test.ntf";
    string fname02 = "tests/tests.ntf";
    GEO::GeoImage geoimage01( fname01, true);
    GEO::GeoImage geoimage02( fname02, true);
    
    // check our validity flags
    note = "getImageTypeName is invalid.";
    if( geoimage01.getImageTypeName() != "NITF" ) return false;
    if( geoimage02.getImageTypeName() != "__NONE__" ) return false;
    
    // check if the data is valid
    note = "isOpenCVCompatible is invalid";
    if( geoimage01.isOpenCVValid() != true ) return false;
    if( geoimage02.isOpenCVValid() != false ) return false;

    // check if the data is valid with opencv 
    note = "isValid is invalid";
    if( geoimage01.isDataValid() != true ) return false;
    if( geoimage02.isDataValid() != false ) return false;

    // return data
    note = "Operation Successful";
    return 1;
}


int TEST_GeoImage_get_image( string& note ){

    // create test GeoImage
    string fname01 = "tests/test.ntf";
    string fname02 = "tests/tests.ntf";
    GEO::GeoImage geoimage01( fname01, true);
    GEO::GeoImage geoimage02( fname02, true);
    

    // get the image data
    Mat img01 = geoimage01.get_image( CV_8UC3 );
    Mat img02 = geoimage02.get_image( CV_8UC3 );

    // check the image size
    note = "Image size is not correct.";
    if( img01.cols != 1024 || img01.rows != 1024 ) return false;
    if( img02.cols != 0    || img02.rows != 0    ) return false;

    // return results
    note = "Successful Operation";
    return 1;

}


