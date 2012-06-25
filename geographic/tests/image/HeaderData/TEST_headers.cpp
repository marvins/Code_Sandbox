#include "TEST_headers.h"

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
int TEST_GeoHeader_filename( string& note );
int TEST_GeoHeader_pixel(    string& note );
int TEST_GeoHeader_driver(   string& note );
int TEST_GeoHeader_header_meta(   string& note );

int TEST_DEMHeader_core(     string& note );
int TEST_ImageHeader_core(   string& note );
int TEST_DTEDHeader_core(    string& note );
int TEST_NITFHeader_core(    string& note );
int TEST_SRTMHeader_core(    string& note );

void TEST_headers(){
    
    print_module_header("GeoHeader and Associated Derived Children");
    
    int result;
    string note;

    /**   GEOHEADER  */
    result = TEST_GeoHeader_filename( note );
    print_test_results( "GeoHeader   filename", result, note );
    
    result = TEST_GeoHeader_pixel( note );
    print_test_results( "GeoHeader   pixel", result, note );
    
    result = TEST_GeoHeader_driver( note );
    print_test_results( "GeoHeader   driver", result, note );
    
    result = TEST_GeoHeader_header_meta( note );
    print_test_results( "GeoHeader   metadata", result, note );

    /**  DEMHeader  */
    result = TEST_DEMHeader_core( note );
    print_test_results( "DEMHeader   core", result, note );
    
    /** ImageHeader */
    result = TEST_ImageHeader_core( note );
    print_test_results( "ImageHeader core", result, note );
    
    /** NITFHeader */
    result = TEST_NITFHeader_core( note );
    print_test_results( "NITFHeader  core", result, note );

    /** DTEDHeader */
    result = TEST_DTEDHeader_core( note );
    print_test_results( "DTEDHeader  core", result, note );
    
    /** SRTM Header */
    result = TEST_SRTMHeader_core( note );
    print_test_results( "SRTMHeader  core", result, note );


    print_module_footer("GeoHeader and Associated Derived Children");

}


int TEST_GeoHeader_filename( string& note ){
    
    GEO::GeoImage img01;
    GEO::GeoImage img02("data/U_1001A.NTF", true );
    
    GEO::GeoHeader_Info* header_data01 = img01.get_header();
    GEO::GeoHeader_Info* header_data02 = img02.get_header();
    
    note = "Operation Failed";
    if( header_data01->isValid() == true ) return false;
    if( header_data02->isValid() == false) return false; 

    if( header_data02->get_image_filename() != "data/U_1001A.NTF") return false;

    note = "Successful Operation";
    return true;
}

int TEST_GeoHeader_pixel(    string& note ){


    note = "Not Implemented";
    return false;
}

int TEST_GeoHeader_driver(   string& note ){


    note = "Not Implemented";
    return false;
}

int TEST_GeoHeader_header_meta(   string& note ){
    
    string filename01 = "data/U_1001A.NTF";
    string filename02 = "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf";
    string filename03 = "data/dted/w119/n038.dt2";
 
    GEO::GeoImage img01( filename01, true);
    GEO::GeoImage img02( filename02, true);
    GEO::GeoImage img03( filename03, true);
    
    GEO::GeoHeader_Info* header_data01 = img01.get_header();
    GEO::GeoHeader_Info* header_data02 = img02.get_header();
    GEO::GeoHeader_Info* header_data03 = img03.get_header();
    
    //make sure headers are valid
    note = "Header is not valid";
    if( header_data01->isValid() == false ) return false;
    if( header_data02->isValid() == false ) return false;
    if( header_data03->isValid() == false ) return false;

    //pull header data
    vector<pair<string,string> > header01 = header_data01->get_header_data();
    vector<pair<string,string> > header02 = header_data02->get_header_data();
    vector<pair<string,string> > header03 = header_data03->get_header_data();
    
    string  value01,  value02,  value03;
    bool   result01, result02, result03;

    note = "Header Item Query Failed";
    result01 = header_data01->get_header_item( "NITF_ICAT", value01 );
    result02 = header_data02->get_header_item( "NITF_ABPP", value02 );
    
    if(  result01 == false || result02 == false || header03.size() != 0 )
        return false;
    
    if( value01 != "VIC" || value02 != "12" ) return false;
    //NITF_ABPP = 12 


    note = "Successful Operation";
    return true;
}

int TEST_DEMHeader_core(     string& note ){
    
    //create DTED Image
    string filename01 = "/data/dted/w119/n036.dt2";


    note = "Not Implemented";
    return false;
}

int TEST_ImageHeader_core(   string& note ){

    note = "Not Implemented";
    return false;
}

int TEST_DTEDHeader_core(    string& note ){

    string filename01 = "/data/dted/w119/n036.dt2";

    //create dted image
    GEO::GeoImage img01( filename01, true );
    
    //pull the dted image header
    GEO::GeoHeader_Info* header01 = img01.get_header();

    //check header characteristics
    note = "isValid() failed";
    if( header01->isValid() != true ) return false;

    note = "get_image_filename failed";
    if( header01->get_image_filename() != filename01 ) return false;

    note = "get_driver_format failed";
    if( header01->get_driver_format() != "DEM.DTED" ) return false;

    note = "Successful Operation";
    return true;
}

int TEST_NITFHeader_core(    string& note ){

    string filename01 = "data/U_1001A.NTF";
    string filename02 = "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf";

    //create dted image
    GEO::GeoImage img01( filename01, true );
    GEO::GeoImage img02( filename02, true );
    
    //pull the dted image header
    GEO::GeoHeader_Info* header01 = img01.get_header();
    GEO::GeoHeader_Info* header02 = img02.get_header();

    //check header characteristics
    note = "isValid() failed";
    if( header01->isValid() != true ) return false;
    if( header02->isValid() != true ) return false;

    note = "get_image_filename failed";
    if( header01->get_image_filename() != filename01 ) return false;
    if( header02->get_image_filename() != filename02 ) return false;

    note = "get_driver_format failed";
    if( header01->get_driver_format() != "IMG.NITF" ) return false;
    if( header02->get_driver_format() != "IMG.NITF" ) return false;

    note = "Successful Operation";
    return true;
}

int TEST_SRTMHeader_core(    string& note ){


    note = "Not Implemented";
    return false;
}

