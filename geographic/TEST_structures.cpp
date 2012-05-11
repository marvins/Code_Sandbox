#include "TEST_structures.h"

#include <fstream>
#include <iostream>
#include <string> 

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <fstream>
#include <iostream>

#include "gdal_priv.h"
#include "cpl_conv.h"

//GeoImage includes
#include <GeoImage.h> 

using namespace cv;
using namespace std;

bool showImg = false;   /*< Flag for viewing the images */

#include "../unit_test/src/Assert.h"
#include "../unit_test/src/Logger.h"
#include "../useful_stuff/string_functions/string_utilities.h"

int TEST_PixelType_types( string& note );
int TEST_PixelType_accessors( string& note );
int TEST_PixelType_convert( string& note );

int TEST_GeoHeader_filename( string& note );
int TEST_GeoHeader_pixel(    string& note );
int TEST_GeoHeader_driver(   string& note );
int TEST_GeoHeader_core(     string& note );

int TEST_GeoImage_core(      string& note );

/** 
 * Beginning of the structures module test
 *
 * Modules covered
 * - PixelType
*/
void TEST_structures_module(){
    
    //if( fabs(GEOVERSION - 1.0) > 0.00001 ){
    //    cout << "BAD VERSION" << endl;
    //}

    /** Framework variables */
    int result;
    string note;

    /**  Test the PixelType Module  */
    print_module_header("PixelType");


    result = TEST_PixelType_types(     note );
    print_test_results( "PixelType  types", result, note );

    result = TEST_PixelType_accessors( note );
    print_test_results( "PixelType  accessors", result, note );
    
    result = TEST_PixelType_convert(   note );
    print_test_results( "PixelType  conversions", result, note );
    
    /**  Test the GeoHeader Module  */
    print_module_header("GeoHeader");

    result = TEST_GeoHeader_core(     note );
    print_test_results(  "GeoHeader  core", result, note );
    
    result = TEST_GeoHeader_filename( note );
    print_test_results(  "GeoHeader  filename", result, note );

    result = TEST_GeoHeader_pixel(    note );
    print_test_results(  "GeoHeader  pixel", result, note );
    
    result = TEST_GeoHeader_driver(   note );
    print_test_results(  "GeoHeader  driver", result, note );
    
    /**  Test the GeoImage Module  */
    print_module_header("GeoImage");
    
    result = TEST_GeoImage_core(      note );
    print_test_results(  "GeoImage   core",  result, note  );

}


/**
 * Test the PixelType internal types
*/
int TEST_PixelType_types( string& note ){
    
    if( PixelType::UNKNOWN != 0 ){
        note = "UNKNOWN Type Failed";
        return 0;
    }
    if( PixelType::UInt8C1 != 1 ){
        note = "UInt8C1 Failed";
        return 0;
    }
    if( PixelType::UInt16C1 != 2 ){
        note = "UInt16C1 Failed";
        return 0;
    }
    if( PixelType::UInt32C1 != 3 ){
        note = "UInt32C1 Failed";
        return 0;
    }

    note = "Successful Operation";
    return true;
}

/** 
 * Test the getters and setters of the PixelType module
*/
int TEST_PixelType_accessors( string& note ){

    //create a couple of pixeltypes 
    PixelType tp01;
    PixelType tp02( PixelType::UNKNOWN  );
    PixelType tp03( PixelType::UInt8C1  );
    PixelType tp04( PixelType::UInt16C1 );
    PixelType tp05( PixelType::UInt32C1 );

    //test get and set
    note = "Get Operation Failed";
    if( tp01.get() != PixelType::UNKNOWN ) return false;
    if( tp02.get() != PixelType::UNKNOWN ) return false;
    if( tp03.get() != PixelType::UInt8C1 ) return false;
    if( tp04.get() != PixelType::UInt16C1) return false;
    if( tp05.get() != PixelType::UInt32C1) return false;

    note = "Set Operation Failed";
    tp01.set( PixelType::UInt32C1 ); if( tp01.get() != PixelType::UInt32C1 ) return false;
    tp02.set( PixelType::UInt16C1 ); if( tp02.get() != PixelType::UInt16C1 ) return false;
    tp03.set( PixelType::UInt8C1  ); if( tp03.get() != PixelType::UInt8C1  ) return false;
    tp04.set( PixelType::UNKNOWN  ); if( tp04.get() != PixelType::UNKNOWN  ) return false;

    note = "Get Name Operation Failed at: ";
    if( tp01.get_name() != "UInt32C1" ){ note += "UInt32C1"; return false; }
    if( tp02.get_name() != "UInt16C1" ){ note += "UInt16C1"; return false; }
    if( tp03.get_name() != "UInt8C1"  ){ note += "UInt8C1";  return false; }
    if( tp04.get_name() != "UNKNOWN"  ){ note += "UNKNOWN";  return false; }

    note = "Successful Operation";
    return true;
}


int TEST_PixelType_convert( string& note ){

    //create a couple of pixeltypes 
    PixelType tp01;
    PixelType tp02( PixelType::UNKNOWN  );
    PixelType tp03( PixelType::UInt8C1  );
    PixelType tp04( PixelType::UInt16C1 );
    PixelType tp05( PixelType::UInt32C1 );
    

    note = "gdal type conversion failed";
    if( tp01.get_gdal_type() != GDT_Unknown ) return false;
    if( tp02.get_gdal_type() != GDT_Unknown ) return false;
    if( tp03.get_gdal_type() != GDT_Byte    ) return false;
    if( tp04.get_gdal_type() != GDT_UInt16  ) return false;
    if( tp05.get_gdal_type() != GDT_UInt32  ) return false;


    note = "Successful Operation";
    return true;
}

int TEST_GeoHeader_filename( string& note ){

    
    note = "Not Implemented";
    return false;
}

int TEST_GeoHeader_pixel(    string& note ){


    note = "Not Implemented";
    return false;
}

int TEST_GeoHeader_driver(   string& note ){


    note = "Not Implemented";
    return false;
}

int TEST_GeoHeader_core(     string& note ){


    note = "Not Implemented";
    return false;
}

int TEST_GeoImage_core(      string& note ){

    

    note = "Not Implemented";
    return false;
}


