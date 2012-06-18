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
int TEST_GS2NITFHeader_core(     string& note );
int TEST_GS2NITFHeader_filename( string& note );
int TEST_GS2NITFHeader_pixel(    string& note );
int TEST_GS2NITFHeader_driver(   string& note );
int TEST_GS2NITFHeader_header_meta(   string& note );

void TEST_extensions_module(){
    
    print_module_header("GeoImage extensions");
    
    int result;
    string note;

    /**   GEOHEADER  */
    result = TEST_GS2NITFHeader_core( note );
    print_test_results( "GS2NITFHeader   core", result, note );
    
    result = TEST_GS2NITFHeader_filename( note );
    print_test_results( "GS2NITFHeader   filename", result, note );
    
    result = TEST_GS2NITFHeader_pixel( note );
    print_test_results( "GS2NITFHeader   pixel", result, note );
    
    result = TEST_GS2NITFHeader_driver( note );
    print_test_results( "GS2NITFHeader   driver", result, note );
    
    result = TEST_GS2NITFHeader_header_meta( note );
    print_test_results( "GS2NITFHeader   metadata", result, note );

    print_module_footer("GeoImage extensions");

}


/** Run the unit test for the Core GeoHeader Module */
int TEST_GS2NITFHeader_core(     string& note ){

    note = "Not Implemented";
    return false;
}


int TEST_GS2NITFHeader_filename( string& note ){
    
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

int TEST_GS2NITFHeader_pixel(    string& note ){


    note = "Not Implemented";
    return false;
}

int TEST_GS2NITFHeader_driver(   string& note ){


    note = "Not Implemented";
    return false;
}

int TEST_GS2NITFHeader_header_meta(   string& note ){
    
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
   
    cout << "outputting metadata" << endl;
    cout << "size is : " << header01.size() << endl;
    for( size_t i=0; i<header01.size(); i++){
        cout << header01[i].first << " = " << header01[i].second << endl;
    }

    note = "Not Fully Implemented";
    return -1;
}

int TEST_GS2NITFHeader_tacid( string& note ){

    note = "Not Implemented";
    return false;
}
