#include "TEST_utilities.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <iostream>

#include <gdal_priv.h>
#include <cpl_conv.h>

//GeoImage includes
#include <GeoImage.h> 

using namespace cv;
using namespace std;

#include <Logger.h>
#include <string_utilities.h>

int TEST_cvDepthChannel2Type( string& note );
int TEST_opencvType2string(   string& note );
int TEST_opencvDepth2string(  string& note );


/** 
 * Beginning of the utilities module test
 *
 * Modules covered
 * - OpenCVUtils
*/
void TEST_utilities_module(){
    
    //if( fabs(GEOVERSION - 1.0) > 0.00001 ){
    //    cout << "BAD VERSION" << endl;
    //}

    /** Framework variables */
    int result;
    string note;

    /**  Test the PixelType Module  */
    print_module_header("OpenCVUtils");


    result = TEST_cvDepthChannel2Type( note );
    print_test_results( "OpenCVUtils   cvDepthChannel2Type", result, note );

    result = TEST_opencvType2string( note );
    print_test_results( "OpenCVUtils   opencvType2string", result, note );
    
    result = TEST_opencvDepth2string(  note );
    print_test_results( "OpenCVUtils   opencvDepth2string", result, note );

    
}


int TEST_cvDepthChannel2Type( string& note ){

    //start running tests
    note = "Operation Failed at: ";
    if( cvDepthChannel2Type( CV_8U, 1 ) != CV_8UC1 ){ note += "CV_8UC1"; return false; }
    if( cvDepthChannel2Type( CV_8U, 2 ) != CV_8UC2 ){ note += "CV_8UC2"; return false; }
    if( cvDepthChannel2Type( CV_8U, 3 ) != CV_8UC3 ){ note += "CV_8UC3"; return false; }
    if( cvDepthChannel2Type( CV_16U, 1 ) != CV_16UC1 ){ note += "CV_16UC1"; return false; }
    if( cvDepthChannel2Type( CV_16U, 2 ) != CV_16UC2 ){ note += "CV_16UC2"; return false; }
    if( cvDepthChannel2Type( CV_16U, 3 ) != CV_16UC3 ){ note += "CV_16UC3"; return false; }
    if( cvDepthChannel2Type( CV_32S, 1 ) != CV_32SC1 ){ note += "CV_32SC1"; return false; }
    if( cvDepthChannel2Type( CV_32S, 2 ) != CV_32SC2 ){ note += "CV_32SC2"; return false; }
    if( cvDepthChannel2Type( CV_32S, 3 ) != CV_32SC3 ){ note += "CV_32SC3"; return false; }


    note = "Successful Operation";
    return true;
}

int TEST_opencvType2string( string& note ){
    
    note = "Failed on CV_8UCX Conversion";
    if( opencvType2string( CV_8UC1 ) != "CV_8UC1" ) return false;
    if( opencvType2string( CV_8UC2 ) != "CV_8UC2" ) return false;
    if( opencvType2string( CV_8UC3 ) != "CV_8UC3" ) return false;

    note = "Failed on CV_16UCX Conversion";
    if( opencvType2string( CV_16UC1 ) != "CV_16UC1" ) return false;
    if( opencvType2string( CV_16UC2 ) != "CV_16UC2" ) return false;
    if( opencvType2string( CV_16UC3 ) != "CV_16UC3" ) return false;

    note = "Failed on CV_32UCX Conversion";
    if( opencvType2string( CV_32SC1 ) != "CV_32SC1" ) return false;
    if( opencvType2string( CV_32SC2 ) != "CV_32SC2" ) return false;
    if( opencvType2string( CV_32SC3 ) != "CV_32SC3" ) return false;
    
    note = "Failed on CV_32FCX Conversion";
    if( opencvType2string( CV_32FC1 ) != "CV_32FC1" ) return false;
    if( opencvType2string( CV_32FC2 ) != "CV_32FC2" ) return false;
    if( opencvType2string( CV_32FC3 ) != "CV_32FC3" ) return false;


    note = "Successful Operation";
    return true;
}

int TEST_opencvDepth2string( string& note ){
    
    note = "Failed on : ";
    if( opencvDepth2string( CV_8U  ) != "CV_8U"  ){ note += "CV_8U";  return false; }
    if( opencvDepth2string( CV_16U ) != "CV_16U" ){ note += "CV_16U"; return false; }
    if( opencvDepth2string( CV_32S ) != "CV_32S" ){ note += "CV_32S"; return false; }
    if( opencvDepth2string( CV_32F ) != "CV_32F" ){ note += "CV_32F"; return false; }
    if( opencvDepth2string( CV_64F ) != "CV_64F" ){ note += "CV_64F"; return false; }

    note = "Successful Operation";
    return true;
}

