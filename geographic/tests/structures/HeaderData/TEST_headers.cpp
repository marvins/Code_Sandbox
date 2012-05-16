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

#include <Assert.h>
#include <Logger.h>
#include <string_utilities.h>

using namespace cv;
using namespace std;

/** GeoHeader_core */
int TEST_GeoHeader_core(     string& note );
int TEST_GeoHeader_filename( string& note );
int TEST_GeoHeader_pixel(    string& note );
int TEST_GeoHeader_driver(   string& note );

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
    result = TEST_GeoHeader_core( note );
    print_test_results( "GeoHeader   core", result, note );
    
    result = TEST_GeoHeader_filename( note );
    print_test_results( "GeoHeader   filename", result, note );
    
    result = TEST_GeoHeader_pixel( note );
    print_test_results( "GeoHeader   pixel", result, note );
    
    result = TEST_GeoHeader_driver( note );
    print_test_results( "GeoHeader   driver", result, note );
    cout << endl;

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


/** Run the unit test for the Core GeoHeader Module */
int TEST_GeoHeader_core(     string& note ){

    note = "Not Implemented";
    return false;
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


int TEST_DEMHeader_core(     string& note ){
    
    //create DTED Image

    note = "Not Implemented";
    return false;
}

int TEST_ImageHeader_core(   string& note ){

    note = "Not Implemented";
    return false;
}

int TEST_DTEDHeader_core(    string& note ){

    string filename01 = "/opt/dted/w119/n036.dt2";

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

/*
int TEST_PixelType_types( string& note );
int TEST_PixelType_accessors( string& note );
int TEST_PixelType_convert( string& note );

int TEST_GeoHeader_filename( string& note );
int TEST_GeoHeader_pixel(    string& note );
int TEST_GeoHeader_driver(   string& note );

int TEST_NITFHeader_inherited( string& note );
int TEST_NITFHeader_core( string& note );

int TEST_GeoImage_core(      string& note );
int TEST_GeoImage_get_image( string& note );

int TEST_GDAL_Data_write(    string& note );
*/

/** 
 * Beginning of the structures module test
 *
 * Modules covered
 * - PixelType
*/
    /**  Test the PixelType Module  */
    //print_module_header("PixelType");


    //result = TEST_PixelType_types(     note );
    //print_test_results( "PixelType  types", result, note );

    //result = TEST_PixelType_accessors( note );
    //print_test_results( "PixelType  accessors", result, note );
    
    //result = TEST_PixelType_convert(   note );
    //print_test_results( "PixelType  conversions", result, note );
    
    /**  Test the GeoHeader Module  */
    //print_module_header("GeoHeader");

    //result = TEST_GeoHeader_core(     note );
    //print_test_results(  "GeoHeader  core", result, note );
    
    //result = TEST_GeoHeader_filename( note );
    //print_test_results(  "GeoHeader  filename", result, note );

    //result = TEST_GeoHeader_pixel(    note );
    //print_test_results(  "GeoHeader  pixel", result, note );
    
    //result = TEST_GeoHeader_driver(   note );
    //print_test_results(  "GeoHeader  driver", result, note );
    
    /**  Test the GeoHeader Module  */
    //print_module_header("NITFHeader");
    
    //result = TEST_NITFHeader_inherited( note );
    //print_test_results(  "NITFHeader inherited functions", result, note );

    //result = TEST_NITFHeader_core( note );
    //print_test_results(  "NITFHeader  core functions", result, note );

    /**  Test the GeoImage Module  */
    //print_module_header( "GeoImage");
    
    //result = TEST_GeoImage_core(      note );
    //print_test_results(  "GeoImage   core",  result, note  );

    //result = TEST_GeoImage_get_image( note );
    //print_test_results(  "GeoImage   get_image", result, note );

    /**  Test the GDAL_Data Module  */
    //print_module_header( "GDAL_Data");

    //result = TEST_GDAL_Data_write( note );
    //print_test_results( "GDAL_Data   write", result, note );



/**
 * Test the PixelType internal types
*
int TEST_PixelType_types( string& note ){
    
    if( GEO::PixelType::UNKNOWN != 0 ){
        note = "UNKNOWN Type Failed";
        return 0;
    }
    if( GEO::PixelType::UInt8C1 != 1 ){
        note = "UInt8C1 Failed";
        return 0;
    }
    if( GEO::PixelType::UInt16C1 != 2 ){
        note = "UInt16C1 Failed";
        return 0;
    }
    if( GEO::PixelType::UInt32C1 != 3 ){
        note = "UInt32C1 Failed";
        return 0;
    }

    note = "Successful Operation";
    return true;
}

 * Test the getters and setters of the PixelType module
*
int TEST_PixelType_accessors( string& note ){

    //create a couple of pixeltypes 
    GEO::PixelType tp01;
    GEO::PixelType tp02( GEO::PixelType::UNKNOWN  );
    GEO::PixelType tp03( GEO::PixelType::UInt8C1  );
    GEO::PixelType tp04( GEO::PixelType::UInt16C1 );
    GEO::PixelType tp05( GEO::PixelType::UInt32C1 );

    //test get and set
    note = "Get Operation Failed";
    if( tp01.get() != GEO::PixelType::UNKNOWN ) return false;
    if( tp02.get() != GEO::PixelType::UNKNOWN ) return false;
    if( tp03.get() != GEO::PixelType::UInt8C1 ) return false;
    if( tp04.get() != GEO::PixelType::UInt16C1) return false;
    if( tp05.get() != GEO::PixelType::UInt32C1) return false;

    note = "Set Operation Failed";
    tp01.set( GEO::PixelType::UInt32C1 ); if( tp01.get() != GEO::PixelType::UInt32C1 ) return false;
    tp02.set( GEO::PixelType::UInt16C1 ); if( tp02.get() != GEO::PixelType::UInt16C1 ) return false;
    tp03.set( GEO::PixelType::UInt8C1  ); if( tp03.get() != GEO::PixelType::UInt8C1  ) return false;
    tp04.set( GEO::PixelType::UNKNOWN  ); if( tp04.get() != GEO::PixelType::UNKNOWN  ) return false;

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
    GEO::PixelType tp01;
    GEO::PixelType tp02( GEO::PixelType::UNKNOWN  );
    GEO::PixelType tp03( GEO::PixelType::UInt8C1  );
    GEO::PixelType tp04( GEO::PixelType::UInt16C1 );
    GEO::PixelType tp05( GEO::PixelType::UInt32C1 );
    

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

int TEST_GeoHeader_core(     string& note ){


    note = "Not Implemented";
    return false;
}


int TEST_NITFHeader_inherited( string& note ){
    
    //create nitf image
    string image_filename = "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf";

    GEO::GeoImage img01;
    GEO::GeoImage img02( image_filename, false);
    GEO::GeoImage img03( image_filename, true );
    GEO::GeoImage img04( image_filename);
    GEO::GeoImage img05;
    img05.set_filename( image_filename );
    img05.set_init(true);

    note = "NITFHeader_Info conversion failed";
    GEO::NITFHeader_Info * header01 = dynamic_cast<GEO::NITFHeader_Info*>(img01.get_header());
    if( header01->isValid() == true  ) return false;

    GEO::NITFHeader_Info * header02 = dynamic_cast<GEO::NITFHeader_Info*>(img02.get_header());
    if( header02->isValid() == true ) return false;
    
    GEO::NITFHeader_Info * header03 = dynamic_cast<GEO::NITFHeader_Info*>(img03.get_header());
    if( header03->isValid() == false ) return false;
    if( header03->get_image_filename() != image_filename ) return false;

    GEO::NITFHeader_Info * header04 = dynamic_cast<GEO::NITFHeader_Info*>(img04.get_header());
    if( header04->isValid() == true ) return false;

    GEO::NITFHeader_Info * header05 = dynamic_cast<GEO::NITFHeader_Info*>(img05.get_header());
    if( header05->isValid() == false ) return false;
    if( header05->get_image_filename() != image_filename ) return false;

    note = "Successful Operation";
    return true;
}

int TEST_NITFHeader_core( string& note ){

    //create nitf image
    string image_filename = "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf";

    GEO::GeoImage img01;
    GEO::GeoImage img02( image_filename, false);
    GEO::GeoImage img03( image_filename, true );
    GEO::GeoImage img04( image_filename);
    GEO::GeoImage img05;
    
    GEO::NITFHeader_Info * header01 = dynamic_cast<GEO::NITFHeader_Info*>(img01.get_header());
    if( header01->get_driver_format() != "NITF" ) return false;
    
    GEO::NITFHeader_Info * header02 = dynamic_cast<GEO::NITFHeader_Info*>(img02.get_header());
    if( header02->get_driver_format() != "NITF" ) return false;
    
    GEO::NITFHeader_Info * header03 = dynamic_cast<GEO::NITFHeader_Info*>(img03.get_header());
    if( header03->get_driver_format() != "NITF" ) return false;
    
    GEO::NITFHeader_Info * header04 = dynamic_cast<GEO::NITFHeader_Info*>(img04.get_header());
    if( header04->get_driver_format() != "NITF" ) return false;
    
    GEO::NITFHeader_Info * header05 = dynamic_cast<GEO::NITFHeader_Info*>(img05.get_header());
    if( header05->get_driver_format() != "NITF" ) return false;
    
    
    
    note = "Successful Operation";
    return true;
}

int TEST_GeoImage_core(      string& note ){

    //create some images
    GEO::GeoImage img01;
    GEO::GeoImage img02("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf", false);
    GEO::GeoImage img03("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf", true );
    GEO::GeoImage img04("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf");
    GEO::GeoImage img05;
    img05.set_filename("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf");

    //start testing
    note = "Image Loading Failed";
    if( img01.get_filename() != "_NO_IMAGE_SELECTED_" ) return false; 
    if( img02.get_filename() != "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf" ) return false; 
    if( img03.get_filename() != "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf" ) return false; 
    if( img04.get_filename() != "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf" ) return false; 
    if( img05.get_filename() != "data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf" ) return false; 

    //test init
    note = "Image get_init() Failed At: ";
    if( img01.get_init() != false ){ note += "img01"; return false; }
    if( img02.get_init() != false ){ note += "img02"; return false; }
    if( img03.get_init() != true  ){ note += "img03"; return false; }
    if( img04.get_init() != false ){ note += "img04"; return false; }
    if( img05.get_init() != false ){ note += "img05"; return false; }

    //test opencv valid or gdal valid
    note = "Validity Flags Failed At: ";
    if( img01.isOpenCVValid() != false ){ note += "img01";  return false; }
    if( img02.isOpenCVValid() != false ){ note += "img02";  return false; }
    img02.set_init( true );
    if( img02.isOpenCVValid() != true  ){ note += "img02";  return false; }
    if( img03.isOpenCVValid() != true  ){ note += "img03";  return false; }
    if( img04.isOpenCVValid() != false ){ note += "img04";  return false; }
    if( img05.isOpenCVValid() != false ){ note += "img05";  return false; }

    note = "Successful Operation";
    return true;
}

int TEST_GeoImage_get_image( string& note ){

    //create some images
    GEO::GeoImage img01;
    GEO::GeoImage img02("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf", false);
    GEO::GeoImage img03("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf", true );
    GEO::GeoImage img04("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf");
    GEO::GeoImage img05;
    img05.set_filename("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf");
    
    note = "Get Image Operation Failed At: ";
    Mat testImg;

    testImg = img01.get_image();
    if( testImg.rows != 0 || testImg.cols != 0 ){ note += "img01"; return false; }

    testImg = img02.get_image();
    if( testImg.rows != 0 || testImg.cols != 0 ){ note += "img02"; return false; }
    
    testImg = img03.get_image();
    if( testImg.rows != 3248 || testImg.cols != 4872 ){ note += "img03"; return false; }
    
    testImg = img04.get_image();
    if( testImg.rows != 0 || testImg.cols != 0 ){ note += "img04"; return false; }
    
    testImg = img05.get_image();
    if( testImg.rows != 0 || testImg.cols != 0 ){ note += "img05"; return false; }

    note = "Operation Successful";
    return true;
}

int TEST_GDAL_Data_write(    string& note ){
    
    //write image
    GEO::GeoImage img01("data/24FEB129Z0200700ZXGEO000GS0000004F482007.ntf", true);
    Mat image01 = img01.get_image();

    GEO::GDAL_Data::write( "data/result01.ntf", image01, img01.get_header()); 
    
    GEO::GeoImage img02("data/result01.ntf", true);
    Mat image02 = img02.get_image();

    note = "Writer Operation Failed";
    if( image01.cols != image02.cols || image01.rows != image02.rows )
        return false;
    
    note = "written image does not equal input image";
    for( int i=0; i<image01.rows; i++ )
        for( int j=0; j<image01.cols; j++)
            if( fabs( image01.at<uchar>(i,j) - image02.at<uchar>(i,j)) > 0.00001 )
                return false;

    note = "Successful Operation";
    return true;
}*/
